// graph-tool -- a general graph modification and manipulation thingy
//
// Copyright (C) 2006-2016 Tiago de Paula Peixoto <tiago@skewed.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "graph_tool.hh"
#include "random.hh"

#include <boost/python.hpp>

#include "graph_blockmodel_overlap_util.hh"
#include "graph_blockmodel_overlap.hh"
#include "graph_blockmodel_multicanonical.hh"
#include "multicanonical_loop.hh"

using namespace boost;
using namespace graph_tool;

GEN_DISPATCH(overlap_block_state, OverlapBlockState, OVERLAP_BLOCK_STATE_params)

template <class State>
GEN_DISPATCH(multicanonical_overlap_block_state,
             Multicanonical<State>::template MulticanonicalBlockState,
             MULTICANONICAL_BLOCK_STATE_params(State))

python::object multicanonical_overlap_sweep(python::object omulticanonical_state,
                                            python::object oblock_state,
                                            rng_t& rng)
{
    python::object ret;
    auto dispatch = [&](auto& block_state)
    {
        typedef typename std::remove_reference<decltype(block_state)>::type
            state_t;

        multicanonical_overlap_block_state<state_t>::make_dispatch
           (omulticanonical_state,
            [&](auto& s)
            {
                auto ret_ = multicanonical_sweep(s, rng);
                ret = python::make_tuple(ret_.first, ret_.second);
            });
    };
    overlap_block_state::dispatch(oblock_state, dispatch);
    return ret;
}

void export_overlap_blockmodel_multicanonical()
{
    using namespace boost::python;
    def("multicanonical_overlap_sweep", &multicanonical_overlap_sweep);
}
