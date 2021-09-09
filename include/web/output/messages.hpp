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

#ifndef WEB_MESSAGES_HPP
#define WEB_MESSAGES_HPP

#include <string>
#include <vector>
#include <external/json.hpp>

using json = nlohmann::json;

namespace cadmium {
	namespace web {
		namespace output {

			template <typename TIME>
			class messages {
			private:
	    		map<TIME, vector<string>> _frames = map<TIME, vector<string>>();

	    		void add_message(TIME t, string message) {
    				get_frames()[t].push_back(message);
	    		}

			public:
	    		map<TIME, vector<string>> &get_frames() { return _frames; }

	    		messages() {}

	    		void add_output_message(TIME t, submodel* component, port* port, string value) {
					string comp_i = std::to_string(component->get_idx());
					string port_i = std::to_string(port->get_idx());
	    			add_message(t, comp_i + "," + port_i + ";" + value);
	    		}

	    		void add_state_message(TIME t, submodel* component, string value) {
					::string comp_i = std::to_string(component->get_idx());
	    			add_message(t, comp_i + ";" + value);
	    		}
			};
		}
	}
}

#endif // WEB_MESSAGES_HPP
