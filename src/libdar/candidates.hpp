/*********************************************************************/
// dar - disk archive - a backup/restoration program
// Copyright (C) 2002-2052 Denis Corbin
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// to contact the author : http://dar.linux.free.fr/email.html
/*********************************************************************/

    /// \file candidates.hpp
    /// \brief helper class for class data_tree to find the list of archive to restore for a given file
    /// \ingroup Private


#ifndef CANDIDATES_HPP
#define CANDIDATES_HPP

#include "../my_config.h"

#include "database_aux.hpp"
#include "archive_num.hpp"
#include <deque>
#include <set>

namespace libdar
{

	/// \ingroup API
	/// @}

    	class candidates
	{
	public:
	    candidates(bool even_when_removed): ewr(even_when_removed) { clear(); };

	    void clear() { num.clear(); status.clear(); };
	    void add(archive_num val, db_etat st);
	    db_lookup get_status() const;
	    void set_the_set(std::set<archive_num> & archive) const;

	private:
	    bool ewr;
	    std::deque<archive_num> num;
	    std::deque<db_etat> status;
	};

	/// @}

} // end of namespace

#endif
