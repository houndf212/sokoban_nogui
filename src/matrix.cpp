/*
 *   Copyright (c) 2007 John Weaver
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include "matrix.h"



/*export*/ template <class T>
Matrix<T>::Matrix() {
  m_rows = 0;
  m_columns = 0;
  m_matrix = nullptr;
}


/*export*/ template <class T>
Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>> init) {
  m_matrix = nullptr;
  m_rows = init.size();
  if ( m_rows == 0 ) {
    m_columns = 0;
  } else {
    m_columns = init.begin()->size();
    if ( m_columns > 0 ) {
      resize(m_rows, m_columns);
    }
  }

  size_t i = 0, j;
  for ( auto row = init.begin() ; row != init.end() ; ++row, ++i ) {
    assert ( row->size() == m_columns && "All rows must have the same number of columns." );
    j = 0;
    for ( auto value = row->begin() ; value != row->end() ; ++value, ++j ) {
      m_matrix[i][j] = *value;
    }
  }
}

/*export*/ template <class T>
Matrix<T>::Matrix(const Matrix<T> &other) {
  if ( other.m_matrix != nullptr ) {
    // copy arrays
    m_matrix = nullptr;
    resize(other.m_rows, other.m_columns);
    for ( size_t i = 0 ; i < m_rows ; i++ ) {
      for ( size_t j = 0 ; j < m_columns ; j++ ) {
        m_matrix[i][j] = other.m_matrix[i][j];
      }
    }
  } else {
    m_matrix = nullptr;
    m_rows = 0;
    m_columns = 0;
  }
}

/*export*/ template <class T>
Matrix<T>::Matrix(const size_t rows, const size_t columns) {
  m_matrix = nullptr;
  resize(rows, columns);
}

/*export*/ template <class T>
Matrix<T> &
Matrix<T>::operator= (const Matrix<T> &other) {
  if ( other.m_matrix != nullptr ) {
    // copy arrays
    resize(other.m_rows, other.m_columns);
    for ( size_t i = 0 ; i < m_rows ; i++ ) {
      for ( size_t j = 0 ; j < m_columns ; j++ ) {
          m_matrix[i][j] = other.m_matrix[i][j];
      }
    }
  } else {
    // free arrays
    for ( size_t i = 0 ; i < m_columns ; i++ ) {
      delete [] m_matrix[i];
    }

    delete [] m_matrix;

    m_matrix = nullptr;
    m_rows = 0;
    m_columns = 0;
  }
  
  return *this;
}

/*export*/

/*export*/

/*export*/

/*export*/


/*export*/
