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

#ifndef WEB_STRUCTURE_MODEL_TYPE_HPP
#define WEB_STRUCTURE_MODEL_TYPE_HPP

#include <string>
#include <vector>
#include <external/json.hpp>

#include <web/output/message_type.hpp>
#include <web/output/port.hpp>
#include <web/output/metadata.hpp>
#include <web/output/port.hpp>
#include <web/output/submodel.hpp>
#include <web/output/int_coupling.hpp>

using json = nlohmann::json;

namespace cadmium {
	namespace web {
		namespace output {
			class model_type{
				private:
					int _idx = -1;
					string _name = "";
					string _type = "";
					message_type* _message_type = NULL;
					metadata* _metadata = NULL;
					vector<port*> _ports = vector<port*>();
					vector<int_coupling*> _internal_couplings = vector<int_coupling*>();
					vector<int> _components = vector<int>();

					map<string, port*> _ports_index;

				public:
					int get_idx() { return _idx; }
					void set_idx(int value) { _idx = value; }

					string get_name() { return _name; }
					void set_name(string value) { _name = value; }

					string get_type() { return _type; }
					void set_type(string value) { _type = value; }

					message_type* get_message_type() { return _message_type; }
					void set_message_type(message_type* value) { _message_type = value; }

					metadata* get_metadata() { return _metadata; }
					void set_metadata(metadata* value) { _metadata = value; }

					vector<port*> &get_ports() { return _ports; }

					vector<int_coupling*> &get_couplings() { return _internal_couplings; }

					vector<int> &get_components() { return _components; }

					map<string, port*> &get_ports_index() { return _ports_index; }

					model_type() {}

					model_type(string _name, string _type, metadata* _metadata) {
						set_name(_name);
						set_type(_type);
						set_metadata(_metadata);
					}

					~model_type() {
						delete _metadata;

						for (port* p : get_ports()) delete p;
						for (int_coupling* ic : get_couplings()) delete ic;
					}

					port* add_port(port* _port) {
						_port->set_idx(get_ports().size());
						get_ports().push_back(_port);
						get_ports_index().insert({ _port->get_name(), _port });

						return _port;
					}

					port* get_port(string name) {
						map<string, port*>::iterator pos = get_ports_index().find(name);

						if (pos == get_ports_index().end()) return NULL;

						else return pos->second;
					}

					void add_coupling(int_coupling* coupling) {
						if (coupling != NULL) get_couplings().push_back(coupling);
					}

					void add_component(submodel* component) {
						if (component != NULL) get_components().push_back(component->get_idx());
					}

					nlohmann::json to_json() {
						json out = {
							{"id", get_idx()},
							{"name", get_name()},
							{"type", get_type()},
							// {"metadata", get_metadata()->to_json() },
							{"ports", json::array()}
						};

						if (get_type() == "atomic" ) out["message_type"] = get_message_type()->get_idx();

						else {
							for (int_coupling* lnk : get_couplings()) out["couplings"].push_back(lnk->to_json());

							for (int c_idx : get_components()) out["components"].push_back(c_idx);
						}

						for (port* port : get_ports()) out["ports"].push_back(port->to_json());

						return out;
					}
			};
		}
	}
}

#endif // WEB_STRUCTURE_MODEL_TYPE_HPP
