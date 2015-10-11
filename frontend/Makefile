default:
	@go fmt *.go
	@go vet *.go
	@go build service.go

lint:
	@golint service.go
	@jslint js/app.js
