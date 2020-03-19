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

#ifndef INCLUDE_LATTICES_TOP_K_PRIORITY_LATTICE_HPP_
#define INCLUDE_LATTICES_TOP_K_PRIORITY_LATTICE_HPP_

#include "core_lattices.hpp"
#include "priority_lattice.hpp"


template <class P, class V> 
class TopKPriorityLattice : public Lattice<std::set<PriorityValuePair<P, V>>> {
  using Element = std::set<PriorityValuePair<P, V>>;
  using Base = Lattice<Element>;

 protected:
  int k_;
  void do_merge(const Element& e) override {
    for (const auto& p: e) {
      this->element.insert(p);
    }

    while (this->element.size() > k_) {
        this->element.erase(this->element.begin());
    }
  
  }

 public:
  TopKPriorityLattice() : Base(Element()) {
    this->k_ = 1;
  }

  TopKPriorityLattice(int k) : Base(Element()) {
    this->k_ = k;
  }

  TopKPriorityLattice(int k, Element e) : Base(e) {
    this->k_ = k;
  }

  MaxLattice<unsigned> size() { return {this->element.size()}; }
};

#endif
