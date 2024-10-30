// This tool is meant to track file changes for recompilation in `blmake`.
// It is NOT a general purpose file watcher, so it has some weird behavior;
// for example, deleted files are not tracked, 
// since they are not needed for recompilation.

package main

import (
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
)

const prevFilePath = "prev.json"
const currFilePath = "curr.json"
const changedFilesPath = "recompile_list.txt"

// FileHashes stores filenames and their hashes.
type FileHashes map[string]string

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: watcher <directory_path>")
		os.Exit(1)
	}
	path := os.Args[1]
	prevHashes := loadPrevHashes()
	currHashes := computeHashes(path)
	latestChanges := make(FileHashes)
	for file, _ := range currHashes {
		if _, ok := prevHashes[file]; ok || len(prevHashes) == 0 {
			if prevHashes[file] != currHashes[file] {
				latestChanges[file] = currHashes[file]
			}
		}
	}
	err := saveHashes(currHashes, latestChanges)
	if err != nil { 
		// No changes
		writeTxtFile(latestChanges) // need to make sure the txt file is empty
		fmt.Println(err)
	} else {
		fmt.Println("Changed files:")
		for file, _ := range latestChanges {
			fmt.Println(file)
		}
		fmt.Println("Files written to 'recompile_list.txt'.")
	}
}

// computeHashes computes SHA-256 hashes for the files in the given directory.
func computeHashes(dir string) FileHashes {
	hashes := make(FileHashes)

	entries, err := os.ReadDir(dir)
	if err != nil {
		log.Fatalf("Error reading directory: %v", err)
	}

	for _, entry := range entries {
		if entry.IsDir() {
			continue
		}

		path := filepath.Join(dir, entry.Name())
		hash, err := hashFile(path)
		if err != nil {
			log.Printf("Error hashing file %s: %v", path, err)
			continue
		}
		hashes[path] = hash
	}

	return hashes
}

// hashFile computes the SHA-256 hash of a file's contents.
func hashFile(filename string) (string, error) {
	f, err := os.Open(filename)
	if err != nil {
		return "", fmt.Errorf("error opening file %s: %w", filename, err)
	}
	defer f.Close()

	hasher := sha256.New()
	if _, err := io.Copy(hasher, f); err != nil {
		return "", fmt.Errorf("error hashing file %s: %w", filename, err)
	}

	return hex.EncodeToString(hasher.Sum(nil)), nil
}

func loadPrevHashes() FileHashes {
	data, err := os.ReadFile(prevFilePath)
	if err != nil {
		if os.IsNotExist(err) {
			return make(FileHashes) // If the file doesn't exist, return an empty map.
		}
		log.Fatalf("Error reading hash file: %v", err)
	}

	var hashes FileHashes
	if err := json.Unmarshal(data, &hashes); err != nil {
		log.Fatalf("Error unmarshaling hash file: %v", err)
	}
	return hashes
}

func saveHashes(currHashes, changedHashes FileHashes) error {
	if len(changedHashes) == 0 {
		// Avoid resetting the JSON if no changes occurred.
		return fmt.Errorf("No changes detected. Keeping previous state.") 
	}
	currData, err := json.MarshalIndent(currHashes, "", "  ")
	if err != nil {
		log.Fatalf("Error marshaling current hashes: %v", err)
	}
	if err := os.WriteFile(prevFilePath, currData, 0644); err != nil {
		log.Fatalf("Error writing prev hash file: %v", err)
	}
	writeTxtFile(changedHashes)
	return nil
}

func writeTxtFile(hashes FileHashes) {
	contents := ""
	for file, _ := range hashes {
		contents += file + "\n"
	}
	if err := os.WriteFile(changedFilesPath, []byte(contents), 0644); err != nil {
		log.Fatalf("Error writing text file: %v", err)
	}
}
