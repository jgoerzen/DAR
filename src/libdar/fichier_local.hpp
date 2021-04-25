/*********************************************************************/
// dar - disk archive - a backup/restoration program
// Copyright (C) 2002-2021 Denis Corbin
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// to contact the author, see the AUTHOR file
/*********************************************************************/

    /// \file fichier_local.hpp
    /// \brief class fichier_local definition. This is a full implementation/inherited class of class fichier_global
    /// this type of object are generated by entrepot_local.
    /// \ingroup Private

#ifndef FICHIER_LOCAL_HPP
#define FICHIER_LOCAL_HPP


#include "../my_config.h"

extern "C"
{
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
} // end extern "C"

#include "integers.hpp"
#include "user_interaction.hpp"
#include "fichier_global.hpp"

#include <string>

namespace libdar
{

	/// \addtogroup Private
	/// @{


	/// filesystem local files

    class fichier_local : public fichier_global
    {
    public :

	    /// full featured constructors
        fichier_local(const std::shared_ptr<user_interaction> & dialog,
		      const std::string & chemin,
		      gf_mode m,
		      U_I permission,
		      bool fail_if_exists,
		      bool erase,
		      bool furtive_mode);

	    /// constructor without user_interaction

	    /// \note can only build a read-only object
	fichier_local(const std::string & chemin, bool furtive_mode = false);

	    /// copy constructor
	fichier_local(const fichier_local & ref) : fichier_global(ref) { copy_from(ref); };

	    /// move constructor
	fichier_local(fichier_local && ref) noexcept: fichier_global(std::move(ref)) { move_from(std::move(ref)); };

	    /// assignment operator
	fichier_local & operator = (const fichier_local & ref) { detruit(); copy_from(ref); return *this; };

	    /// move operator
	fichier_local & operator = (fichier_local && ref) noexcept { fichier_global::operator = (std::move(ref)); move_from(std::move(ref)); return *this; };

	    /// destructor
	~fichier_local() { detruit(); };


	    /// set the ownership of the file
	virtual void change_ownership(const std::string & user, const std::string & group) override;

	    /// change the permission of the file
	virtual void change_permission(U_I perm) override;

	    /// return the size of the file
        virtual infinint get_size() const override;

	    /// set posix_fadvise for the whole file
	virtual void fadvise(advise adv) const override;

            // inherited from generic_file
	virtual bool skippable(skippability direction, const infinint & amount) override { return true; };
        virtual bool skip(const infinint & pos) override;
        virtual bool skip_to_eof() override;
        virtual bool skip_relative(S_I x) override;
	virtual bool truncatable(const infinint & pos) const override { return true; };
        virtual infinint get_position() const override;

	    /// provide the low level filedescriptor to the call and terminate()

	    /// \note this is the caller duty to close() the provided filedescriptor
	S_I give_fd_and_terminate() { int ret = filedesc; filedesc = -1; terminate(); return ret; };

    protected :
	    // inherited from generic_file grand-parent class
	virtual void inherited_truncate(const infinint & pos) override;
	virtual void inherited_read_ahead(const infinint & amount) override {}; // nothing done, calling readahead(2) could be added in the future
	virtual void inherited_sync_write() override { fsync(); };
	virtual void inherited_flush_read() override {}; // nothing stored in transit in this object
	virtual void inherited_terminate() override { if(adv == advise_dontneed) fadvise(adv); };

	    // inherited from fichier_global parent class
	virtual U_I fichier_global_inherited_write(const char *a, U_I size) override;
        virtual bool fichier_global_inherited_read(char *a, U_I size, U_I & read, std::string & message) override;

    private :
        S_I filedesc;
	advise adv;

	void open(const std::string & chemin,
		  gf_mode m,
		  U_I permission,
		  bool fail_if_exists,
		  bool erase,
		  bool furtive_mode);

	void copy_from(const fichier_local & ref);
	void move_from(fichier_local && ref) noexcept;
	void detruit() { if(filedesc >= 0) close(filedesc); filedesc = -1; };
	int advise_to_int(advise arg) const;

	    /// sync the data to disk

	    /// this is necessary under Linux for better efficiency
	    /// before calling fadvise(advise_dontneed) else write pending blocks
	    /// would stay in the cache more time than necessary
	void fsync() const;

	off_t get_eof_offset() const;

    };

	/// @}

} // end of namespace

#endif
