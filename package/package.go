package csort

import (
	denv "github.com/jurgen-kluft/ccode/denv"
	ccore "github.com/jurgen-kluft/ccore/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'csort'
func GetPackage() *denv.Package {
	// Dependencies
	cunittestpkg := cunittest.GetPackage()
	ccorepkg := ccore.GetPackage()

	// The main (csort) package
	mainpkg := denv.NewPackage("github.com\\jurgen-kluft", "csort")
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(ccorepkg)

	// 'csort' library
	mainlib := denv.SetupCppLibProject(mainpkg, "csort")
	mainlib.AddDependencies(ccorepkg.GetMainLib()...)

	// 'csort' unittest project
	maintest := denv.SetupCppTestProject(mainpkg, "csort"+"_test")
	maintest.AddDependencies(cunittestpkg.GetMainLib()...)
	maintest.AddDependency(mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
