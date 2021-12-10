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

#ifndef WEB_LOGGER_HPP
#define WEB_LOGGER_HPP

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <NDTime.hpp>
#include <cadmium/modeling/dynamic_message_bag.hpp>
#include <cadmium/logger/logger.hpp>
#include <cadmium/logger/common_loggers.hpp>
#include <cadmium/engine/common_helpers.hpp>

namespace cadmium {
    namespace web {
		template<typename TIME>
		struct formatter : cadmium::logger::formatter<TIME> {
			static std::string sim_messages_collect(const TIME& t, const std::string& model_id, const std::string& outbox) {
				std::ostringstream oss;
				if (outbox.length() > 0) oss << model_id << "|" << outbox;
				return oss.str();
			};

			static std::string sim_state(const TIME& t, const std::string& model_id, const std::string& model_state) {
				std::ostringstream oss;
				oss << model_id << "|" << model_state;
				return oss.str();
			};

			static TIME run_global_time(const TIME& global_time) {
				return global_time;
			}

			static std::string run_info(const std::string& message) {
				return message;
			}
		};

		template<typename T>
		std::ostream& implode(std::ostream& os, const T& collection){
			auto it = std::begin(collection);
			if (it != std::end(collection)) {
				cadmium::logger::value_or_name<typename T::value_type>::print(os, *it);
				++it;
			}
			while (it != std::end(collection)){
				os << ";";
				cadmium::logger::value_or_name<typename T::value_type>::print(os, *it);
				++it;
			}
			return os;
		}

		template<typename BST>
		void print_dynamic_messages_by_port(std::ostream& os, cadmium::dynamic::message_bags& bags) {
			int a = 0; // to dynamically count tuple index

			auto print_messages_fold_expr = [&a, &bags, &os](auto b) -> void {
				using port_type = decltype(b);
				using bag_type = typename cadmium::message_bag<port_type>;

				// there is no messages in the port
				if (bags.find(typeid(port_type)) == bags.cend()) return;

				bag_type& casted_bag = boost::any_cast<bag_type&>(bags.at(typeid(port_type)));

				if (casted_bag.messages.size() > 0) {
					if (a != 0)  os << "|";

					const char* name = boost::typeindex::type_id<port_type>().name();
					std::string pretty = boost::core::demangle(name);
					os << pretty.substr(pretty.find_last_of(':') + 1) << "|";

					web::implode(os, casted_bag.messages);

					a++;
				}
			};

			BST bs;
			cadmium::helper::for_each<BST>(bs, print_messages_fold_expr);
		}

		template<typename LOGGER_SOURCE, class FORMATTER, typename SINK_PROVIDER>
		struct logger{
			template<typename DECLARED_SOURCE, typename EVENT, typename... PARAMs>
			static void log(const PARAMs&... ps) {
				if constexpr (std::is_same<LOGGER_SOURCE, DECLARED_SOURCE>::value) {
					if constexpr (std::is_same<EVENT, cadmium::logger::coor_info_init>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_info_init(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_info_collect>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_info_collect(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_routing_collect>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_routing_collect(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_routing_collect_ic>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_routing_collect_ic(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_routing_collect_eic>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_routing_collect_eic(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_routing_collect_eoc>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_routing_collect_eoc(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_info_advance>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_info_advance(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_routing_ic_collect>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_routing_ic_collect(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_routing_eic_collect>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_routing_eic_collect(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::coor_routing_eoc_collect>::value) {
						SINK_PROVIDER::sink() << FORMATTER::coor_routing_eoc_collect(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::sim_info_init>::value) {
						SINK_PROVIDER::sink() << FORMATTER::sim_info_init(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::sim_state>::value) {
						SINK_PROVIDER::sink() << FORMATTER::sim_state(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::sim_info_collect>::value) {
						SINK_PROVIDER::sink() << FORMATTER::sim_info_collect(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::sim_messages_collect>::value) {
						std::string message = FORMATTER::sim_messages_collect(ps...);

						if (message.length() == 0) return;

						SINK_PROVIDER::sink() << message;
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::sim_info_advance>::value) {
						SINK_PROVIDER::sink() << FORMATTER::sim_info_advance(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::sim_local_time>::value) {
						SINK_PROVIDER::sink() << FORMATTER::sim_local_time(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::run_global_time>::value) {
						SINK_PROVIDER::sink() << FORMATTER::run_global_time(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}

					if constexpr (std::is_same<EVENT, cadmium::logger::run_info>::value) {
						SINK_PROVIDER::sink() << FORMATTER::run_info(ps...);
						SINK_PROVIDER::sink() << std::endl;
					}
				}
			}
		};

		// Define default output files
	    static std::ofstream out_messages("../simulation_results/output_messages.txt");

	    struct oss_sink_messages {
	        static std::ostream& sink() {
	            return out_messages;
	        }
	    };

	    static std::ofstream out_state("../simulation_results/state_messages.txt");

	    struct oss_sink_state {
	        static std::ostream& sink() {
	            return out_state;
	        }
	    };

	    using state = web::logger<cadmium::logger::logger_state, web::formatter<NDTime>, oss_sink_state>;
	    using log_messages = web::logger<cadmium::logger::logger_messages, web::formatter<NDTime>, oss_sink_messages>;
	    using global_time_mes = web::logger<cadmium::logger::logger_global_time, web::formatter<NDTime>, oss_sink_messages>;
	    using global_time_sta = web::logger<cadmium::logger::logger_global_time, web::formatter<NDTime>, oss_sink_state>;

	    using logger_top = cadmium::logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;
	}
}

#endif // WEB_LOGGER_HPP
