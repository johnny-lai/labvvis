XCODEBUILD_CMD = xcodebuild -scheme labvvis -workspace ./labvvis.xcworkspace -configuration Deployment

default: build

build:
	$(XCODEBUILD_CMD) build

clean:
	$(XCODEBUILD_CMD) clean
