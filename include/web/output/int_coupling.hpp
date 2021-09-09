/**
 * Copyright (c) 2017, Bruno St-Aubin
 * Carleton University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WEB_STRUCTURE_INT_COUPLING_HPP
#define WEB_STRUCTURE_INT_COUPLING_HPP

#include <string>
#include <vector>
#include <external/json.hpp>

using json = nlohmann::json;

namespace cadmium {
	namespace web {
		namespace output {
			class int_coupling{
			private:
				int _from_component_idx = -1;
				int _from_port_idx = -1;
				int _to_component_idx = -1;
				int _to_port_idx = -1;

			public:
				int get_from_component_idx() { return _from_component_idx; }
				void set_from_component_idx(int value) { _from_component_idx = value; }

				int get_from_port_idx() { return _from_port_idx; }
				void set_from_port_idx(int value) { _from_port_idx = value; }

				int get_to_component_idx() { return _to_component_idx; }
				void set_to_component_idx(int value) { _to_component_idx = value; }

				int get_to_port_idx() { return _to_port_idx; }
				void set_to_port_idx(int value) { _to_port_idx = value; }

				int_coupling() {}

				int_coupling(int _from_component_idx, int _from_port_idx, int _to_component_idx, int _to_port_idx) {
					set_from_component_idx(_from_component_idx);
					set_from_port_idx(_from_port_idx);
					set_to_component_idx(_to_component_idx);
					set_to_port_idx(_to_port_idx);
				}

				~int_coupling() {

				}

				nlohmann::json to_json() {
					json out = json::array();

					out.push_back(get_from_component_idx());
					out.push_back(get_from_port_idx());
					out.push_back(get_to_component_idx());
					out.push_back(get_to_port_idx());

					return out;
				}
			};
		}
	}
}

#endif // WEB_STRUCTURE_INT_COUPLING_HPP
