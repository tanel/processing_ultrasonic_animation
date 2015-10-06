default:
	@go fmt service.go
	@go vet service.go
	@go build service.go

lint:
	@golint service.go
	@jslint js/app.js
