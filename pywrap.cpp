#include "MGML.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(MGML, m) {
	m.doc() = "pybind11 plugin"; // optional module docstring

	m.def("MCBM",	&MGML::MCBM);
	m.def("MWBM",	&MGML::MWBM);
	m.def("MCM",	&MGML::MCM);
}
