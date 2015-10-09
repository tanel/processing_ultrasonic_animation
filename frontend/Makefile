default:
	@go fmt *.go
	@go vet *.go
	@go build service.go
	@go build multipart_upload.go

lint:
	@golint service.go
	@jslint js/app.js
