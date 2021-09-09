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

#ifndef WEB_STRUCTURE_METADATA_HPP
#define WEB_STRUCTURE_METADATA_HPP

#include <string>
#include <vector>
#include <external/json.hpp>

using json = nlohmann::json;

namespace cadmium {
	namespace web {
		namespace output {
			class metadata{
			private:
				string _description = "";
				string _date_created = "";
				int _author = -1;
				vector<string> _tags = {};

			public:
				string get_description() { return _description; }
				void set_description(string value) { _description = value; }

				string get_date_created() { return _date_created; }
				void set_date_created(string value) { _date_created = value; }

				int get_author() { return _author; }
				void set_author(int value) { _author = value; }

				vector<string> get_tags() { return _tags; }
				void set_tags(vector<string> value) { _tags = value; }

				metadata() {}

				metadata(string _description, string _date_created, int _author) {
					set_description(_description);
					set_date_created(_date_created);
					set_author(_author);
				}

				void add_tag(string tag) {
					get_tags().push_back(tag);
				}

				nlohmann::json to_json() {
					return {
						{"description", get_description()},
						{"created", get_date_created()},
						{"author", get_author()},
						{"tags", get_tags()}
					};
				}
			};
		}
	}
}

#endif // WEB_STRUCTURE_METADATA_HPP
