/*
 * Copyright 2010  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*! ---------------------------------------------------------------
 *
 *
 * \file MarkTableDelegat.cpp
 * \brief MarkTableDelegat implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "MarkTableDelegat.h"

namespace GUI
{
  /**
   * Find a child element by an attribute value.
   *
   * TODO: write description
   *
   * @param        el              Element containing children to search.
   *
   * @return        @li     The first child element that matches the
   *                        attribute.
   *                @li     NULL if no child was found with an attribute
   *                        of the specified value.
   */

  MarkTableDelegat::MarkTableDelegat(QWidget *parent ) : QItemDelegate(parent)
  {
  }

  MarkTableDelegat::~MarkTableDelegat()
  {
  }

  int MarkTableDelegat::columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
  {
    return 2;
  }

  //struct xml_element *
  //find_xml_element_by_attr(struct xml_element *el, const char *el_name,
  //    const char *attr_name, const char *attr_val)

}                                       // namespace GUI


/* ===[ End of file  ]=== */
