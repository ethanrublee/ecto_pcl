/*
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "ecto_pcl.hpp"
#include <pcl/filters/extract_indices.h>

#define DECLAREEXTRACTINDICES(r, data, i, ELEM)                            \
  BOOST_PP_COMMA_IF(i) pcl::ExtractIndices< BOOST_PP_TUPLE_ELEM(2, 0, ELEM) >

typedef boost::variant< BOOST_PP_SEQ_FOR_EACH_I(DECLAREEXTRACTINDICES, ~, ECTO_XYZ_POINT_TYPES) > filter_variant_t;

#include "filters/Filter.hpp"

struct ExtractIndices
{
  template <typename Point>
  struct filter {
    typedef typename ::pcl::ExtractIndices<Point> type;
  };

  static void declare_params(ecto::tendrils& params)
  {
    pcl::ExtractIndices<pcl::PointXYZ> default_;
    params.declare<bool> ("negative", "Sets whether the indices should be returned, or all points _except_ the indices.", default_.getNegative());
  }
  static void declare_io(const tendrils& params, tendrils& inputs, tendrils& outputs) {
    inputs.declare<indices_t::ConstPtr> ("indices", "The indices to extract.");
  }

  ExtractIndices() {}

  template <typename Point>
  void configure(pcl::ExtractIndices<Point>& f)
  {
    f.setNegative(negative);
  }
  void configure(tendrils& params, tendrils& inputs, tendrils& outputs)
  {
    negative = params.get<bool> ("negative");
    indices_ = inputs.at("indices");
  }

  template <typename Point>
  int process(pcl::ExtractIndices<Point>& f)
  {
    f.setIndices(*indices_);
    return 0;
  }
  int process(const tendrils& inputs, tendrils& outputs){ return 0; }

  ecto::spore<indices_t::ConstPtr> indices_;
  bool negative;

};

ECTO_CELL(ecto_pcl, ecto::pcl::FilterCell<ExtractIndices>, "ExtractIndices", "...");

