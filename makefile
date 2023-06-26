
.PHONY: r

b:
	cmake --build build

r: b
	build/main