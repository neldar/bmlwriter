/*
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Copyright (C) 2010 Michael Richter (alias neldar)                         *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation; either version 3 of the License, or (at your    *
 * option) any later version.                                                *
 *                                                                           *
 * This program is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General  *
 * Public License for more details.                                          *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, see <http://www.gnu.org/licenses/>.            *
 *                                                                           *
 *---------------------------------------------------------------------------*
*/

#ifndef BMLWRITERERROR_H_
#define BMLWRITERERROR_H_


#define SUCCESS					0

#define PROGRAM_ERROR_BASE			1
#define ERROR_INVALID_ARGC			PROGRAM_ERROR_BASE
#define ERROR_SRC_OPEN				PROGRAM_ERROR_BASE + 1
#define ERROR_DST_OPEN				PROGRAM_ERROR_BASE + 2
#define ERROR_UNLOCK_FAIL			PROGRAM_ERROR_BASE + 3

#define COPY_ERROR_BASE				10
#define	ERROR_FSTAT				COPY_ERROR_BASE
#define	ERROR_READ				COPY_ERROR_BASE + 1
#define	FATAL_READ_PARTIAL_WRITE		COPY_ERROR_BASE + 2
#define ERROR_PARTIAL_READ			COPY_ERROR_BASE + 3
#define FATAL_PARTIAL_READ_PARTIAL_WRITE	COPY_ERROR_BASE + 4
#define FATAL_WRITE				COPY_ERROR_BASE + 5


#endif
