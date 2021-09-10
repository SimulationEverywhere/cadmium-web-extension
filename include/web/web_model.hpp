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

#ifndef WEB_MODEL_HPP
#define WEB_MODEL_HPP

#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/engine/pdevs_dynamic_link.hpp>
#include <cadmium/modeling/dynamic_message_bag.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/basic_model/pdevs/iestream.hpp>

#include <web/output/message_type.hpp>
#include <web/web_logger.hpp>
#include <web/web_tools.hpp>

using namespace cadmium::web::output;

namespace cadmium {
    namespace web {

		// *** USING STATEMENTS REPLACEMENT *** //
		using EICs = cadmium::dynamic::modeling::EICs;
		using EOCs = cadmium::dynamic::modeling::EOCs;
		using ICs = cadmium::dynamic::modeling::ICs;
		using Models = cadmium::dynamic::modeling::Models;
		using Ports = cadmium::dynamic::modeling::Ports;

		using model = dynamic::modeling::model;

		// typedef dynamic::modeling::model model;

		// *** COMMON MODEL BASE CLASS *** //
		class web_extension {
		private:
			string _m_class = "";

		private:
            virtual vector<string> get_port_names(vector<type_index> ports) {
            	vector<string> names = {};

				for (type_index port : ports) names.push_back(tools::clean_name(port.name()));

            	return names;
            };


		public:
			virtual string get_id() = 0;

			virtual string get_class() = 0;

			virtual string get_type() = 0;

			virtual message_type get_state_message_type() = 0;

            virtual map<string, message_type> get_output_ports_message_types() = 0;

            virtual map<string, message_type> get_input_ports_message_types() = 0;

			virtual vector<shared_ptr<cadmium::dynamic::modeling::model>> get_models() = 0;

			virtual vector<cadmium::dynamic::modeling::EIC> get_eics() = 0;

			virtual vector<cadmium::dynamic::modeling::EOC> get_eocs() = 0;

			virtual vector<cadmium::dynamic::modeling::IC> get_ics() = 0;

			virtual ~web_extension() {}
		};

		// *** MODEL REPLACEMENTS *** //
		template<template<typename T> class ATOMIC, typename TIME, typename... Args>
		class atomic : public cadmium::dynamic::modeling::atomic<ATOMIC, TIME, Args...>, public web_extension {

		private:
			string _m_class = "";

			template <typename PORTS>
			map<string, message_type> get_ports(vector<type_index> raw_names) {
				int i = 0;
				PORTS ports;
				map<string, message_type> mts;

                cadmium::helper::for_each<PORTS>(ports, [&i, &raw_names, &mts] (auto &p) -> void {
		    		string name = tools::clean_name(raw_names[i++].name());

                	mts.insert({ name, p.get_message_type() });
                });

				return mts;
			}

		public:
            using model_type=ATOMIC<TIME>;

			using output_ports = typename cadmium::dynamic::modeling::atomic<ATOMIC, TIME, Args...>::output_ports;
			using input_ports = typename cadmium::dynamic::modeling::atomic<ATOMIC, TIME, Args...>::input_ports;

			atomic() :
				cadmium::dynamic::modeling::atomic<ATOMIC, TIME, Args...>() { }

			atomic(const string& model_id, string m_class, Args&&... args) :
				cadmium::dynamic::modeling::atomic<ATOMIC, TIME, Args...>(model_id, forward<Args>(args)...)  {
				_m_class = m_class;
			}

			string get_id() override {
				return cadmium::dynamic::modeling::atomic<ATOMIC, TIME, Args...>::get_id();
			};

			string get_class() override {
				return _m_class;
			}

			string get_type() override {
				return "atomic";
			}

			message_type get_state_message_type() override {
                // Forwards the translated value to the wrapped model_type class method.
                return model_type::get_state_message_type();
			};

			map<string, message_type> get_output_ports_message_types() override {
            	vector<type_index> raw_names = cadmium::dynamic::modeling::atomic<ATOMIC, TIME, Args...>::get_output_ports();

            	return get_ports<output_ports>(raw_names);
            };

			map<string, message_type> get_input_ports_message_types() override {
            	vector<type_index> raw_names = cadmium::dynamic::modeling::atomic<ATOMIC, TIME, Args...>::get_input_ports();

            	return get_ports<input_ports>(raw_names);
            };

			vector<shared_ptr<cadmium::dynamic::modeling::model>> get_models() override {
				return {};
			};

			vector<cadmium::dynamic::modeling::EIC> get_eics() override {
				return {};
			};

			vector<cadmium::dynamic::modeling::EOC> get_eocs() override {
				return {};
			};

			vector<cadmium::dynamic::modeling::IC> get_ics() override {
				return {};
			};

			string messages_by_port_as_string(cadmium::dynamic::message_bags outbox) const override {
				ostringstream oss;
				web::print_dynamic_messages_by_port<output_ports>(oss, outbox);
				return oss.str();
			};
		};

		template<typename TIME>
		class coupled : public cadmium::dynamic::modeling::coupled<TIME>, public web_extension{

		private:
			string _m_class = "";

		public:
			coupled():
				cadmium::dynamic::modeling::coupled<TIME>() { };

			coupled(string model_id, string m_class) :
				cadmium::dynamic::modeling::coupled<TIME>(model_id)  {
				_m_class = m_class;
			}

			coupled(string id, string m_class, Models models, Ports input_ports,  Ports output_ports, EICs eic, EOCs eoc, ICs ic):
				cadmium::dynamic::modeling::coupled<TIME>(id, models, input_ports, output_ports, eic, eoc, ic) {
				_m_class = m_class;
			}

			string get_id() override {
				return cadmium::dynamic::modeling::coupled<TIME>::get_id();
			};

			string get_class() {
				return _m_class;
			}

			virtual string get_type() override {
				return "coupled";
			}

			// This is super awkward, it needs an implementation but never
			// gets called. There's likely a better way to do this.
			message_type get_state_message_type() override {
				return message_type();
			};

			map<string, message_type> get_output_ports_message_types() override {
            	return map<string, message_type>();
            };

			map<string, message_type> get_input_ports_message_types() override {
            	return map<string, message_type>();
            };

			vector<shared_ptr<cadmium::dynamic::modeling::model>> get_models() override {
				return this->_models;
			};

			vector<cadmium::dynamic::modeling::EIC> get_eics() override {
				return this->_eic;
			};

			vector<cadmium::dynamic::modeling::EOC> get_eocs() override {
				return this->_eoc;
			};

			vector<cadmium::dynamic::modeling::IC> get_ics() override {
				return this->_ic;
			};
		};

		template<typename TIME>
		class top_web : public web::coupled<TIME> {

		private:
			string _class = "";

		public:
			top_web():
				web::coupled<TIME>() { };

			top_web(string _id, string _m_class) :
				web::coupled<TIME>(_id, _m_class)  { }

			top_web(string id, string m_class, Models models, Ports input_ports,  Ports output_ports, EICs eic, EOCs eoc, ICs ic):
				web::coupled<TIME>(id, m_class, models, input_ports, output_ports, eic, eoc, ic) { }

			virtual string get_type() {
				return "top";
			}
		};

		// *** Make function replacements *** //
		template<template <typename T> typename ATOMIC, typename TIME, typename... Args >
		shared_ptr<cadmium::web::atomic<ATOMIC, TIME, Args...>> make_atomic_model(const string& model_id, string m_class, Args&&... args) {
			return make_shared<cadmium::web::atomic<ATOMIC, TIME, Args...>>(model_id, m_class, forward<Args>(args)...);
		}

		template<typename TIME>
		shared_ptr<cadmium::web::coupled<TIME>> make_coupled_model(string id, string m_class, Models models, Ports i_ports, Ports o_ports, EICs eics, EOCs eocs, ICs ics) {
		    return make_shared<cadmium::web::coupled<TIME>>(id, m_class, models, i_ports, o_ports, eics, eocs, ics);
		}

		template<typename TIME>
		shared_ptr<cadmium::web::top_web<TIME>> make_top_model(string id, string m_class, Models models, Ports i_ports, Ports o_ports, EICs eics, EOCs eocs, ICs ics) {
			return make_shared<cadmium::web::top_web<TIME>>(id, m_class, models, i_ports, o_ports, eics, eocs, ics);
		}

        template<typename PORT_FROM, typename PORT_TO>
        cadmium::dynamic::modeling::EOC make_EOC(std::string model_from) {
        	return cadmium::dynamic::translate::make_EOC<PORT_FROM, PORT_TO>(model_from);
        }

        template<typename PORT_FROM, typename PORT_TO>
        cadmium::dynamic::modeling::EIC make_EIC(std::string model_to) {
        	return cadmium::dynamic::translate::make_EIC<PORT_FROM, PORT_TO>(model_to);
        }

        template<typename PORT_FROM, typename PORT_TO>
        cadmium::dynamic::modeling::IC make_IC(string model_from, string model_to) {
        	return cadmium::dynamic::translate::make_IC<PORT_FROM, PORT_TO>(model_from, model_to);
        }

		// *** RUNNER REPLACEMENT *** //
		template<typename TIME, typename LOGGER>
		using runner = dynamic::engine::runner<TIME, LOGGER>;
	}
}

#endif // WEB_MODEL_HPP
