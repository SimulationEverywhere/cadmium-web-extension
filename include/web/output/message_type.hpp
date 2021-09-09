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

#ifndef WEB_STRUCTURE_MESSAGE_TYPE_HPP
#define WEB_STRUCTURE_MESSAGE_TYPE_HPP

#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <external/json.hpp>

using json = nlohmann::json;

namespace cadmium {
	namespace web {
		namespace output {
			class message_type{
			private:
				int _idx = -1;
				std::string _id = "";
				std::vector<std::string> _fields {};
				std::string _description = "";

			public:
				int get_idx() { return _idx; }
				void set_idx(int value) { _idx = value; }

				std::string get_id() { return _id; }
				void set_id(std::string value) { _id = value; }

				std::vector<std::string> get_fields() { return _fields; }
				void set_fields(std::vector<std::string> value) { _fields = value; }

				std::string get_description() { return _description; }
				void set_description(std::string value) { _description = value; }

				message_type() {}

				message_type(std::string _id, std::vector<std::string> _fields, std::string _description) {
					set_id(_id);
					set_fields(_fields);
					set_description(_description);
				}

				~message_type() {

				}

				std::string get_template() {
					return "[\"" + boost::algorithm::join(get_fields(), "\",\"") + "\"]";
				}

				json to_json() {
					return {
						{"id", get_idx()},
						{"name", get_id()},
						{"template", get_fields()},
						{"description", get_description()}
					};
				}
			};
		}
	}
}

#endif // WEB_STRUCTURE_MESSAGE_TYPE_HPP
