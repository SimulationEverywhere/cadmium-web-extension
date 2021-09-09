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

#ifndef WEB_STRUCTURE_PORT_HPP
#define WEB_STRUCTURE_PORT_HPP

#include <string>
#include <vector>
#include <external/json.hpp>

using json = nlohmann::json;

namespace cadmium {
	namespace web {
		namespace output {
			class port{
			private:
				int _idx = -1;
				string _name;
				string _type;
				int _message_type_idx = -1;

			public:
				int get_idx() { return _idx; }
				void set_idx(int value) { _idx = value; }

				string get_name() { return _name; }
				void set_name(string value) { _name = value; }

				string get_type() { return _type; }
				void set_type(string value) { _type = value; }

				int get_message_type_idx() { return _message_type_idx; }
				void set_message_type_idx(int value) { _message_type_idx = value; }

				port() {}

				port(string _name, string _type) {
					set_name(_name);
					set_type(_type);
				}

				~port() {

				}

				nlohmann::json to_json() {
					return {
						{"name", get_name()},
						{"type", get_type()},
						{"message_type", get_message_type_idx()}
					};
				}
			};
		}
	}
}

#endif // WEB_STRUCTURE_PORT_HPP
