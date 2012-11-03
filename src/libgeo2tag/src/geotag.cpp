/*
 * Copyright 2010 - 2012  Kirill Krinkin  kirill.krinkin@gmail.com
 *
 * Geo2tag LBS Platform (geo2tag.org)
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
 * \file Geotag.cpp
 * \brief Geotag implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "../inc/geotag.h"
#include <cmath>

namespace Geo
{

Geotag::Geotag() : m_coord(.0,.0,.0), m_timestamp(now())
{

}

Geotag::Geotag(double b, double l, double h, const char * label, const char *data) :
    m_coord(b,l,h),m_label(label),m_data(data)
{
}

time_t Geotag::timestamp() const
{
    return m_timestamp;
}

bool Geotag::isColocated(const Geotag &obj, double distancePrecision, double anglePrecision)
{
    if(fabs(altitude()-obj.altitude())>=distancePrecision)
        return false;
    if(fabs(longitude()-obj.longitude())>=anglePrecision)
        return false;
    if(fabs(latitude()-obj.latitude())>=anglePrecision)
        return false;

    return true;
}

time_t Geotag::now()
{
    return time(NULL);
}

} // namespace Geo2tag
