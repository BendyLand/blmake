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

// FileHashes stores filenames and their hashes.
type FileHashes map[string]string

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: watcher <directory_path>")
		os.Exit(1)
	}
	path := os.Args[1]

	// Load previous hashes from the JSON file.
	prevHashes := loadPrevHashes()

	// Compute current hashes of the files in the directory.
	currHashes := computeHashes(path)

	// Store the latest changes.
	latestChanges := make(FileHashes)
	for file, _ := range currHashes {
		if _, ok := prevHashes[file]; ok || len(prevHashes) == 0 {
			if prevHashes[file] != currHashes[file] {
				latestChanges[file] = currHashes[file]
				delete(prevHashes, file)
			}
		}
	}

	// Save the merged changes to the JSON file.
	err := saveHashes(latestChanges)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("Hashes updated.")
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

// loadHashes loads the file hashes from the JSON file.
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

// saveHashes saves the merged file hashes to the JSON file.
func saveHashes(hashes FileHashes) error {
	if len(hashes) == 0 {
		return fmt.Errorf("No changes detected. Keeping previous state.") // Avoid resetting the JSON if no changes occurred.
	}
	data, err := json.MarshalIndent(hashes, "", "  ")
	if err != nil {
		log.Fatalf("Error marshaling hashes: %v", err)
	}
	if err := os.WriteFile(currFilePath, data, 0644); err != nil {
		log.Fatalf("Error writing curr hash file: %v", err)
	}
	if err := os.WriteFile(prevFilePath, data, 0644); err != nil {
		log.Fatalf("Error writing prev hash file: %v", err)
	}
	return nil
}
