package main

import (
	ccode "github.com/jurgen-kluft/ccode"
	cpkg "github.com/jurgen-kluft/csort/package"
)

func main() {
	ccode.Init()
	ccode.GenerateClangFormat()
	ccode.GenerateGitIgnore()
	ccode.Generate(cpkg.GetPackage())
}
