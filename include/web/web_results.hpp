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

#ifndef WEB_RESULTS_HPP
#define WEB_RESULTS_HPP

#include <string>
#include <vector>

#include <web/web_model.hpp>
#include <web/web_tools.hpp>
#include <web/output/structure.hpp>
#include <web/output/messages.hpp>

namespace cadmium {
    namespace web {

		template<typename FUNC>
		static void line_by_line(string path, FUNC&& f) {
			ifstream file;
			file.open(path);
			string l;

			while (std::getline(file, l)) f(l);
		}

		template<typename TIME>
		static void generate_output_messages(structure* s, cadmium::web::output::messages<TIME>* msgs, string path) {
			TIME t;

			line_by_line(path, [&s, &msgs, &t] (auto &l) -> void {
				vector<string> sp = tools::split(l, '|');

				// A time line is length 1. Any other would be odd length (model|port|message|port|message...).
				if (sp.size() % 2 == 0) throw std::runtime_error( "Badly formed output message line. Split size is incorrect." );

				else if (sp.size() == 1) t = TIME(l);

				else {
					submodel* m = s->get_component(sp[0]);

					for (int i = 1; i < sp.size(); i = i + 2) {
						vector<string> sub = tools::split(sp[i + 1], ';');
						port* p = s->get_port(sp[0], sp[i]);

						for (int j = 0; j < sub.size(); j++) msgs->add_output_message(t, m, p, sub[j]);
					}
				}
			});
		}

		template<typename TIME>
		static void generate_state_messages(structure* s, cadmium::web::output::messages<TIME>* msgs, string path) {
			TIME t;

			line_by_line(path, [&s, &msgs, &t] (auto &l) -> void {

				vector<string> split = tools::split(l, '|');

				if (split.size() == 1) t = TIME(l);

				else {
					submodel* comp = s->get_component(split[0]);

					for (int i = 1; i < split.size(); i++) {
						msgs->add_state_message(t, comp, split[i]);
					}
				}
			});
		}

		template<typename TIME>
    	static void output_messages(cadmium::web::output::messages<TIME>* msgs, string folder) {
    		std::ofstream o_messages(folder + "messages.log");

    	    if (!o_messages.is_open()) cerr << "Unable to open messages.log file";

    	    else {
        		for (auto const& frame : msgs->get_frames()) {
        			o_messages << frame.first << endl;

        			for (auto const& message : frame.second) {
        				o_messages << message << endl;
        			}
        		}

        		o_messages.close();
    	    }
    	}

    	static void output_structure(structure* s, string folder) {
    		ofstream o_structure(folder + "structure.json");

    	    if (!o_structure.is_open()) cerr << "Unable to open structure.json file";

    	    else {
    	    	o_structure << s->to_json() << endl;
    	    	o_structure.close();
    	    }
    	}

		static void traverse_model(structure* s, cadmium::dynamic::modeling::model* p_model, void (*fn)(structure* s, web_extension* p_ext) ){
			web_extension* p_ext = dynamic_cast<web_extension*> (p_model);
			// TODO: p_ext can be null

			fn(s, p_ext);

			for (auto& p_submodel : p_ext->get_models()) {
				traverse_model(s, p_submodel.get(), fn);
			}
		}

		template<typename TIME>
        static structure make_structure(shared_ptr<web::coupled<TIME>> p_top, string formalism) {
        	structure s(formalism, "Cadmium");

    		try {
    			traverse_model(&s, p_top.get(), [] (structure* s, web_extension* p_ext) -> void {
    				s->add_model_type(p_ext);
    			});

    			traverse_model(&s, p_top.get(), [] (structure* s, web_extension* p_ext) -> void {
    				s->add_component(p_ext);
    			});

    			traverse_model(&s, p_top.get(), [] (structure* s, web_extension* p_ext) -> void {
    				s->add_model_type_components(p_ext);
    			});

    			traverse_model(&s, p_top.get(), [] (structure* s, web_extension* p_ext) -> void {
    				s->add_couplings(p_ext);
    			});

    			s.set_top(s.get_component(p_top->get_id())->get_idx());
    		}
    		catch(std::exception& e) {
    			std::cout << e.what();

        		return s;
    		}

    		return s;
    	}

		template<typename TIME>
        static void output_results(shared_ptr<web::coupled<TIME>> p_top, string formalism, string path) {
		    cadmium::web::output::messages<TIME> msgs;

			web::output::structure s = make_structure(p_top, formalism);
		    web::output_structure(&s, path);
		    web::generate_output_messages(&s, &msgs, path + "output_messages.txt");
		    web::generate_state_messages(&s, &msgs, path + "state_messages.txt");
		    web::output_messages(&msgs, path);
    	}

		static structure make_cell_devs_structure(std::vector<std::string>fields, string config, string folder) {
			std::ifstream ifs(config);
			json j = json::parse(ifs);

        	string name = j.at("cells").at("default").at("cell_type").get<string>();
        	vector<int> dim = j.at("shape").get<vector<int>>();

        	structure s("Cell-DEVS", "Cadmium");

        	message_type * m_top = s.add_message_type(message_type("s_top", { "out" }, "No description available."));
        	message_type * m_grid = s.add_message_type(message_type("s_grid", fields, "No description available."));

        	model_type * mt_top = s.add_model_type(new model_type("top", "coupled", new metadata()));
        	model_type * mt_grid = s.add_model_type(new model_type(name, "coupled", new metadata()));

        	mt_grid->set_dim(dim);

        	mt_top->set_message_type(s.get_message_type("s_top"));
        	mt_grid->set_message_type(s.get_message_type("s_grid"));

        	for (int i = 0; i < fields.size(); i++) mt_grid->add_port(new port(fields[i], "output"));

        	mt_top->add_port(new port("out", "output"));

        	submodel * sm_top = s.add_component(new submodel("top", mt_top->get_idx()));
        	submodel * sm_grid = s.add_component(new submodel(name, mt_grid->get_idx()));

        	mt_top->add_component(sm_grid);

        	s.set_top(s.get_component(name)->get_idx());

        	return s;
		}

		static vector<string> generate_cell_devs_state_messages(string path) {
			vector<string> messages = vector<string>();

			line_by_line(path, [&messages] (auto &l) -> void {
				vector<string> sp = tools::split(l, ' ');

				if (sp.size() != 1 && sp.size() != 6) throw std::runtime_error( "Badly formed output message line. Split size is incorrect." );

				else if (sp.size() == 1) messages.push_back(l);

				else {
					int s = sp[3].find("(") + 1;
					int e = sp[3].find(")");

					string coord = sp[3].substr(s, e - s);
					string value = sp[5];

					if (web::tools::split(coord, ',').size() == 2) coord = coord + ",0";

					messages.push_back(coord + ";" + value);
				}
			});

			return messages;
		}

		template<typename TIME>
    	static void output_cell_devs_messages(cadmium::web::output::messages<TIME>* msgs, string folder) {
    		std::ofstream o_messages(folder + "messages.log");

    	    if (!o_messages.is_open()) cerr << "Unable to open messages.log file";

    	    else {
        		for (auto const& frame : msgs->get_frames()) {
        			o_messages << frame.first << endl;

        			for (auto const& message : frame.second) {
        				o_messages << message << endl;
        			}
        		}

        		o_messages.close();
    	    }
    	}

		static void make_default_style(string folder) {
			nlohmann::json j_styles = {{
				{ "buckets", {{
					{ "start", 0 },
					{ "end", 100 },
					{ "color", { 150, 40, 110 } }
				}}
			}}};

			std::ofstream o_style(folder + "style.json");

    	    if (!o_style.is_open()) cerr << "Unable to open messages.log file";

    	    else {
    	    	o_style << j_styles << endl;
    	    	o_style.close();
    	    }
		}

		static void convert_cell_devs(std::vector<std::string>fields, string config, string results, string folder) {
		    web::output::structure s = make_cell_devs_structure(fields, config, folder);
		    web::output_structure(&s, folder);

		    vector<string> messages = web::generate_cell_devs_state_messages(results);

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

#endif // WEB_RESULTS_HPP
