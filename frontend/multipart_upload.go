package main

import (
	"bytes"
	"flag"
	"fmt"
	"io"
	"log"
	"mime/multipart"
	"net/http"
	"os"
	"path/filepath"
)

var filename = flag.String("filename", "", "file to upload")

// Creates a new file upload http request with optional extra params
func newfileUploadRequest(uri string, params map[string]string, paramName, path string) (*http.Request, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	body := &bytes.Buffer{}
	writer := multipart.NewWriter(body)
	part, err := writer.CreateFormFile(paramName, filepath.Base(path))
	if err != nil {
		return nil, err
	}
	_, err = io.Copy(part, file)

	for key, val := range params {
		_ = writer.WriteField(key, val)
	}
	err = writer.Close()
	if err != nil {
		return nil, err
	}

	return http.NewRequest("POST", uri, body)
}

func main() {
	flag.Parse()
	if len(*filename) == 0 {
		log.Fatal("missing filename")
		os.Exit(1)
	}
	extraParams := map[string]string{}
	request, err := newfileUploadRequest("http://ox.linnagalerii.ee/upload.php", extraParams, "upfile", *filename)
	if err != nil {
		log.Fatal(err)
		os.Exit(1)
	}
	client := &http.Client{}
	resp, err := client.Do(request)
	if err != nil {
		log.Fatal(err)
		os.Exit(1)
	}
	body := &bytes.Buffer{}
	resp.Body.Close()
	_, err = body.ReadFrom(resp.Body)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(resp.StatusCode)
	fmt.Println(resp.Header)

	fmt.Println(body)
}
