//  Copyright 2019 U.C. Berkeley RISE Lab
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef INCLUDE_CLIENT_KVS_MOCK_CLIENT_HPP_
#define INCLUDE_CLIENT_KVS_MOCK_CLIENT_HPP_

#include "anna.pb.h"
#include "client/kvs_client.hpp"
#include "common.hpp"
#include "requests.hpp"
#include "threads.hpp"
#include "types.hpp"

class KvsMockClient : public KvsClientInterface {
 public:
  /**
   * @addrs A vector of routing addresses.
   * @routing_thread_count The number of thread sone ach routing node
   * @ip My node's IP address
   * @tid My client's thread ID
   * @timeout Length of request timeouts in ms
   */
  KvsMockClient() { rid_ = 0; }

  ~KvsMockClient() {}

  /**
   * Issue an async PUT request to the KVS for a certain lattice typed value.
   */
  string put_async(const Key& key, const string& payload, LatticeType lattice_type) {
      keys_put_.push_back(key);
      last_request_type_ = RequestType::PUT;
      values_[key] = {payload, lattice_type};
      last_rid_ = get_request_id();
      return last_rid_;
  }

  /**
   * Issue an async GET request to the KVS.
   */
  void get_async(const Key& key) {
      keys_get_.push_back(key);
      last_request_type_ = RequestType::GET;
  }

  vector<KeyResponse> receive_async() {
      KeyResponse response;
      if(last_request_type_ == RequestType::PUT){
          response.set_type(RequestType::PUT);
          response.set_response_id(last_rid_);
          auto* tp = response.add_tuples();
          tp->set_error(AnnaError::NO_ERROR);
      }
      if(last_request_type_ == RequestType::GET){
          response.set_type(RequestType::GET);
          KeyTuple *tp = response.add_tuples();
          tp->set_key(keys_get_.back());
          pair<string, LatticeType> value = values_[keys_get_.back()];
          tp->set_payload(value.first);
          tp->set_lattice_type(value.second);
      }
      vector<KeyResponse> responses;
      responses.push_back(response);
      return responses;
  }

  zmq::context_t* get_context() { return nullptr; }

  void clear() {
    keys_put_.clear();
    keys_get_.clear();
    values_.clear();
  }

  // keep track of the keys being put
  vector<Key> keys_put_;
  // keep track of the keys being get
  vector<Key> keys_get_;
  // // responses to send back
  // vector<KeyResponse> responses_;
  // map from keys to values
  map<Key, pair<string, LatticeType>> values_;
  // type of last request for client to know what to send back
  RequestType last_request_type_;
  // track last rid
  string last_rid_;

 private:
  /**
   * Generates a unique request ID.
   */
  string get_request_id() {
    if (++rid_ % 10000 == 0) rid_ = 0;
    return std::to_string(rid_++);
  }

  // the current request id
  unsigned rid_;
};

#endif  // INCLUDE_CLIENT_KVS_MOCK_CLIENT_HPP_
