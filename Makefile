all:
	cd plugin-dev-pre; make; cd ../plugin-dev; make; cd ../plugin-dev2; make
clean:
	cd plugin-dev-pre; rm main
