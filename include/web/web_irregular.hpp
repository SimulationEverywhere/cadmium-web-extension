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

#ifndef WEB_IRREGULAR_HPP
#define WEB_IRREGULAR_HPP

#include <string>
#include <vector>

#include <web/web_model.hpp>
#include <web/web_tools.hpp>
#include <web/web_results.hpp>
#include <web/output/structure.hpp>
#include <web/output/messages.hpp>

namespace cadmium {
    namespace web {

		static structure make_irregular_structure(string main_name, std::vector<std::string>fields, string config, string folder) {
			std::ifstream ifs(config);
			json j = json::parse(ifs);

        	structure s("DEVS", "Cadmium");

        	message_type * m_top = s.add_message_type(message_type("s_top", { "out" }, "No description available."));
        	message_type * m_grid = s.add_message_type(message_type("s_grid", fields, "No description available."));

        	model_type * mt_top = s.add_model_type(new model_type("top", "coupled", new metadata()));
        	model_type * mt_main = s.add_model_type(new model_type(main_name, "coupled", new metadata()));

        	mt_top->set_message_type(s.get_message_type("s_top"));
        	mt_main->set_message_type(s.get_message_type("s_grid"));

        	for (int i = 0; i < fields.size(); i++) mt_main->add_port(new port(fields[i], "output"));

        	mt_top->add_port(new port("out", "output"));

        	submodel * sm_top = s.add_component(new submodel("top", mt_top->get_idx()));
        	
			for (auto& el : j.at("cells").items()) {
				if (el.key() == "default") continue;
				
				submodel * sm_main = s.add_component(new submodel(el.key(), mt_main->get_idx()));
				
				mt_top->add_component(sm_main);
			}

        	s.set_top(s.get_component("top")->get_idx());

        	return s;
		}

		static vector<string> generate_irregular_state_messages(string main_name, structure* s, string path) {
            vector<string> messages = vector<string>();

            ifstream file;
            file.open(path);
            string l;

            while (std::getline(file, l)) {
                vector<string> sp = tools::split(l, ' ');

                if (sp.size() != 1 && sp.size() != 6) throw std::runtime_error( "Badly formed output message line. Split size is incorrect." );

                else if (sp.size() == 1) messages.push_back(l);

                else {
                    int i = main_name.length() + 1;

                    string model = sp[3].substr(i);
                    string value = sp[5];

                    int idx = s->get_component(model)->get_idx();

                    messages.push_back(to_string(idx) + ";" + value);
                }
			};

            return messages;
		}

		static void convert_irregular(string name, std::vector<std::string>fields, string config, string results, string folder) {
		    web::output::structure s = make_irregular_structure(name, fields, config, folder);
		    web::output_structure(&s, folder);
			
		    vector<string> messages = generate_irregular_state_messages(name, &s, results);

			std::ofstream o_messages(folder + "messages.log");

			if (!o_messages.is_open()) cerr << "Unable to open messages.log file";

			else {
				for (auto const& line : messages) o_messages << line << endl;

				o_messages.close();
			}

			make_default_style(folder);
		}
    }
}

#endif // WEB_IRREGULAR_HPP
