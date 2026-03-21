# Makefile

# ==================================================================================== #
# ENVIRONMENT & VARIABLES
# ==================================================================================== #

ECHO_PREFIX = [make]

# ==================================================================================== #
# HELPERS
# ==================================================================================== #

## help: Print this help message
.PHONY: help
help:
	@echo 'Usage:'
	@sed -n 's/^##//p' ${MAKEFILE_LIST} | column -t -s ':' | sed -e 's/^/ /'

# ==================================================================================== #
# DEVELOPMENT
# ==================================================================================== #

## run/keyboard: Run the cmd/controller application in keyboard mode
.PHONY: run/keyboard
run/keyboard:
	go run ./cmd/controller -mode=keyboard

## run/gamepad: Run the cmd/controller application in gamepad mode
.PHONY: run/gamepad
run/gamepad:
	go run ./cmd/controller -mode=gamepad

## run/test/gamepad-input: Run the cmd/gamepad-input program to test gamepad
.PHONY: run/test/gamepad-input
run/test/gamepad-input:
	go run ./cmd/gamepad-input

# ==================================================================================== #
# QUALITY CONTROL
# ==================================================================================== #

## tidy: Tidy module dependencies and format all .go files
.PHONY: tidy
tidy:
	@echo '${ECHO_PREFIX} Tidying module dependencies...'
	go mod tidy
	@echo '${ECHO_PREFIX} Verifying and vendoring module dependencies...'
	go mod verify
# 	go mod vendor
	@echo '${ECHO_PREFIX} Formatting .go files...'
	go fmt ./...

## audit: Run quality control checks and tests
.PHONY: audit
audit:
	@echo '${ECHO_PREFIX} Checking module dependencies...'
	go mod tidy -diff
	go mod verify
	@echo '${ECHO_PREFIX} Vetting code...'
	go vet ./...
# 	go tool staticcheck ./...
	@echo '${ECHO_PREFIX} Running tests...'
	go test -race -vet=off ./...

# ==================================================================================== #
# BUILD
# ==================================================================================== #

## build/controller: Build the cmd/controller application
.PHONY: build/controller
build/controller:
	@echo '${ECHO_PREFIX} Building cmd/controller...'
	go build -ldflags='-s' -o=./bin/controller ./cmd/controller
