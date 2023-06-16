package main

import (
	"fmt"
	"log"

	"github.com/Wind-River/extract-go"
)

func main() {
	fmt.Printf("Sanity\n")

	extractor, err := extract.NewAt("src/dates.tar.bz2", "dates.tar.bz2", "dst")
	if err != nil {
		log.Fatal(err)
	}

	if err := extractor.Enclose(); err != nil {
		log.Fatal(err)
	}

	if extracted, err := extractor.Extract(); err != nil {
		log.Fatal(err)
	} else {
		log.Printf("extracted: %s\n", extracted)
	}
}
