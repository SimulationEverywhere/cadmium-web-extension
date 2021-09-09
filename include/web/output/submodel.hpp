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

#ifndef WEB_STRUCTURE_SUBMODEL_HPP
#define WEB_STRUCTURE_SUBMODEL_HPP

#include <string>
#include <vector>
#include <external/json.hpp>

using json = nlohmann::json;

namespace cadmium {
	namespace web {
		namespace output {

			class submodel {
			private:
				int _idx = -1;
				string _model_id = "";
				int _model_type_idx = -1;

			public:
				int get_idx() { return _idx; }
				void set_idx(int value) { _idx = value; }

				string get_model_id() { return _model_id; }
				void set_model_id(string value) { _model_id = value; }

				int get_model_type_idx() { return _model_type_idx; }
				void set_model_type_idx(int value) { _model_type_idx = value; }

				submodel() {}

				submodel(string _model_id) {
					set_model_id(_model_id);
				}

				// string model type wo we can store it on first pass and convert after.
				submodel(string _model_id, int _model_type_idx): submodel(_model_id) {
					set_model_type_idx(_model_type_idx);
				}

				nlohmann::json to_json() {
					return {
						{"id", get_model_id()},
						{"model_type", get_model_type_idx()}
					};
				}
			};
		}
	}
}

#endif // WEB_STRUCTURE_SUBMODEL_HPP
