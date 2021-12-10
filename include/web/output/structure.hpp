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

#ifndef WEB_STRUCTURE_HPP
#define WEB_STRUCTURE_HPP

#include <string>
#include <vector>
#include <external/json.hpp>

#include <web/output/submodel.hpp>
#include <web/output/model_type.hpp>
#include <web/output/message_type.hpp>

using json = nlohmann::json;

namespace cadmium {
	namespace web {
		namespace output {
			class structure{
				private:
					string _formalism;
					string _simulator;
					int _top = -1;
					vector<model_type*> _model_types = vector<model_type*>();
					vector<message_type*> _message_types = vector<message_type*>();
					vector<submodel*> _components = vector<submodel*>();

					map<string, model_type*> _model_types_index;
					map<string, message_type*> _message_types_index;
					map<string, submodel*> _components_index;

				public:
					string get_formalism() { return _formalism; }
					void set_formalism(string value) { _formalism = value; }

					string get_simulator() { return _simulator; }
					void set_simulator(string value) { _simulator = value; }

					int get_top() { return _top; }
					void set_top(int value) { _top = value; }

					vector<model_type*> &get_model_types() { return _model_types; }

					vector<message_type*> &get_message_types() { return _message_types; }

					vector<submodel*> &get_components() { return _components; }

					map<string, model_type*> &get_model_types_index() { return _model_types_index; }

					map<string, message_type*> &get_message_types_index() { return _message_types_index; }

					map<string, submodel*> &get_components_index() { return _components_index; }

					structure() {}

					structure(string _formalism, string _simulator) {
						set_formalism(_formalism);
						set_simulator(_simulator);
					}

					~structure() {
						for (model_type* mt : get_model_types()) delete mt;
						for (message_type* mt : get_message_types()) delete mt;
						for (submodel* sm : get_components()) delete sm;
					}

					message_type* get_message_type(string type) {
						map<string, message_type*>::iterator pos = get_message_types_index().find(type);

						if (pos == get_message_types_index().end()) return NULL;

						else return pos->second;
					}

					message_type* add_message_type(message_type msg) {
						message_type* p_msg = get_message_type(msg.get_id());

						if (p_msg) return p_msg;

						p_msg = new message_type(msg.get_id(), msg.get_fields(), msg.get_description());
						p_msg->set_idx(get_message_types().size());

						get_message_types().push_back(p_msg);
						get_message_types_index().insert({ p_msg->get_id(), p_msg });

						return p_msg;
					}

					submodel* get_component(string id) {
						map<string, submodel*>::iterator pos = get_components_index().find(id);

						if (pos == get_components_index().end()) return NULL;

						else return pos->second;
					}

					submodel* add_component(web_extension* p_ext) {
						model_type* mt = get_model_type(p_ext->get_class());

						return add_component(new submodel(p_ext->get_id(), mt->get_idx()));
					}

					submodel* add_component(submodel* component) {
						component->set_idx(get_components().size());
						get_components().push_back(component);
						get_components_index().insert({ component->get_model_id(), component });

						return component;
					}

					void add_model_type_components(web_extension* p_ext) {
						model_type* mt = get_model_type(p_ext->get_class());

						for (auto& p_component : p_ext->get_models()) {
							mt->add_component(get_component(p_component->get_id()));
						}
					}

					model_type* get_model_type(string type) {
						map<string, model_type*>::iterator pos =get_model_types_index().find(type);

						if (pos == get_model_types_index().end()) return NULL;

						else return pos->second;
					}

					port* get_port(string model_id, string port) {
			    		submodel* c = get_component(model_id);
			    		model_type* mt = get_model_types().at(c->get_model_type_idx());

			    		return mt->get_port(port);
					}

					void add_ports(model_type* p_mt, string type, map<string, message_type> ports_map) {
						// Add all input and output ports
						for (pair<string, message_type> kv: ports_map) {
							port* p_port = p_mt->add_port(new port(kv.first, type));
							message_type* p_msg = add_message_type(kv.second);
							p_port->set_message_type_idx(p_msg->get_idx());
						};
					}

					int_coupling* make_coupling(string mA, string mB, std::shared_ptr<cadmium::dynamic::engine::link_abstract> link) {
						string pA = tools::clean_name(link->from_port_type_index().name());
						string pB = tools::clean_name(link->to_port_type_index().name());

						submodel* from_model = get_component(mA);
						submodel* to_model = get_component(mB);

						if (from_model == NULL || to_model == NULL) return NULL;

						port* from_port = get_port(mA, pA);
						port* to_port = get_port(mB, pB);

						if (from_port == NULL || to_port == NULL) return NULL;

			    		return new int_coupling(from_model->get_idx(), from_port->get_idx(), to_model->get_idx(), to_port->get_idx());
					}

					void add_couplings(web_extension* p_ext) {
						// Check if model_type already exists
						model_type* p_mt = get_model_type(p_ext->get_class());

						for (cadmium::dynamic::modeling::EOC l : p_ext->get_eocs()) {
							p_mt->add_coupling(make_coupling(l._from, p_ext->get_id(), l._link));
						}

						for (cadmium::dynamic::modeling::EIC l : p_ext->get_eics()) {
							p_mt->add_coupling(make_coupling(p_ext->get_id(), l._to, l._link));
						}

						for (cadmium::dynamic::modeling::IC l : p_ext->get_ics()) {
							p_mt->add_coupling(make_coupling(l._from, l._to, l._link));
						}
					}

					model_type* add_model_type(web_extension* p_ext) {
						// Check if model_type already exists
						model_type* p_mt = get_model_type(p_ext->get_class());

						if (p_mt) return p_mt;

						// Model type doesn't already exist, must be created from model extension
						p_mt = add_model_type(new model_type(p_ext->get_class(), p_ext->get_type(), new metadata()));

						// Atomic models have a state message type, it must be added to structure
						// if it's not already there. Then it must be assigned to the model type.
						// NOTE: Can't figure out how to avoid the string check...
						if (p_ext->get_type() == "atomic") {
							message_type* p_msg = add_message_type(p_ext->get_state_message_type());
							p_mt->set_message_type(p_msg);
						}

						add_ports(p_mt, "input", p_ext->get_input_ports_message_types());
						add_ports(p_mt, "output", p_ext->get_output_ports_message_types());

						return p_mt;
					}

					model_type* add_model_type(model_type* p_mt) {
						// Add built model to structure, set idx, etc.
						p_mt->set_idx(get_model_types().size());
						get_model_types().push_back(p_mt);
						get_model_types_index().insert({ p_mt->get_name(), p_mt });

						return p_mt;
					}

					nlohmann::json to_json() {
						json j_structure = {
							{"simulator", get_simulator()},
							{"formalism", get_formalism()},
							{"top", get_top()},
							{"model_types", json::array()},
							{"message_types", json::array()}
						};

						for (model_type* mt : get_model_types()) {
							j_structure["model_types"].push_back(mt->to_json());
						}

						for (message_type* mt : get_message_types()) {
							j_structure["message_types"].push_back(mt->to_json());
						}

						for (submodel* sm : get_components()) {
							j_structure["components"].push_back(sm->to_json());
						}

						return j_structure;
					}
			};
		}
	}
}

#endif // WEB_STRUCTURE_HPP
