/**
 * @file    HashTable.h
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#ifndef WIN32
#pragma interface
#endif // WIN32


#include <cassert>
#include <vector>

using std::vector ;

typedef unsigned int uint;

enum { MAX_PRIME = 28 };

static const uint prime[MAX_PRIME] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
  1610612741ul, 3221225473ul, 4294967291ul
};


template <typename _KEY,typename _DATA>
class HCell ;

template <typename _KEY,typename _DATA>
class Hash_it ;

//_____________________________________________________________________________
//Class HashTable

template <typename _KEY,typename _DATA>

class HashTable
//-----------------------------------------------------------------------------
{
public:
  typedef Hash_it<_KEY,_DATA> iterator ;

protected:
  typedef HCell<_KEY,_DATA> cell ;  
  friend class Hash_it<_KEY,_DATA> ;

public:
	// Constructor & Desctructor  
  HashTable ( ) : m_size(0) {} 

  HashTable ( const int& size ) : m_size(0) { m_buckets.reserve(size);} 

  ~HashTable	() { clear();}

  //------------------------------------------------------
  //Operators
public:
  friend bool operator== (const HashTable& ht1, const HashTable& ht2)
  {  
    if (ht1.m_buckets.size() != ht2.m_buckets.size()) return false;
    for (uint i = 0; i < (uint)ht1.m_buckets.size(); ++i) 
    {
      cell* cur1 = ht1.m_buckets[i];
      cell* cur2 = ht2.m_buckets[i];
      for ( ; cur1 && cur2 && cur1->m_data == cur2->m_data;
            cur1 = cur1->m_next, cur2 = cur2->m_next)
        {}
      if (cur1 || cur2) return false;
    }
    return true;
  }

  friend bool operator!= (const HashTable& ht1, const HashTable& ht2)
  { return !(ht1==ht2); }


  //------------------------------------------------------
  //Accessories
public:
  void reserve( const uint& size ) { m_buckets.reserve(size); }
  const uint size() const { return m_size; }
  const bool empty() const { return m_size==0; }

  iterator begin() const
  { 
    if( m_size == 0 ) return end();
    for (uint i = 0; i < m_buckets.size(); ++i)
      if (m_buckets[i])
        return iterator(m_buckets[i],this);
    return end();
  }

  iterator end() const { return iterator(NULL,this); }

  iterator at( const _KEY& k ) const
  {
    cell* tmp = hasher(k);
    if( tmp )
    {
      if(k == tmp->m_key ) return iterator( tmp , this );
      else if(tmp->m_next)
      {
        while( tmp && (k != tmp->m_key) )
          tmp = tmp->m_next ;
        return iterator( tmp , this );
      }
    }    
    return end();
  }
  
  iterator insert(const _KEY& k , const _DATA& data )
  {
    const uint n = convert(k) ;
    cell* first = hasher(k) ;
    for (cell* cur = first; cur; cur = cur->m_next)
      if ( k == cur->m_key )
        return iterator( cur , this ) ;

    cell* tmp = new cell( k , data , first) ;
    if( n >= m_buckets.size() ) m_buckets.resize( n + 1000 ) ;    
    m_buckets[n] = tmp;
    ++m_size;
    return iterator(tmp, this) ;
  } 
  
  iterator erase(const iterator& it)
  {
    cell* del = it.m_cell ;
    if (del) 
    {
      const uint n = convert(del->m_key);
      cell* cur = m_buckets[n];
      if ( cur == del) 
      {
        m_buckets[n] = cur->m_next;
        if( m_buckets[n] )
        {
          delete cur ;
          --m_size ;
          return iterator(m_buckets[n],this);
        }
        else
        {
          iterator next = ++iterator(cur,this);
           delete cur ;
          --m_size ;
          return next;
        }
      }
      else 
      {
        cell* next = cur->m_next;
        while (next) 
        {
          if ( next == del) 
          {
            cur->m_next = next->m_next;
            delete next ;
            --m_size;
            return iterator(cur, this) ;
            break;
          }
          else 
          {
            cur  = next;
            next = cur->m_next;
          }
        }
      }
    }
    return end();
  }  

  void clear() 
  {
    for (uint i = 0; i < (uint)m_buckets.size(); ++i) 
    {    
      cell* cur = m_buckets[i];
      while (cur != NULL) 
      {
        cell* next = cur->m_next;
        delete cur; cur = NULL ;
        cur = next;
      }
      m_buckets[i] = NULL;
    }
    m_size = 0;
    m_buckets.clear(); 
  }
  
protected:
  const uint elems_in_bucket(const uint& n ) const
  {
    uint result = 0;
    for ( cell* cur = m_buckets[n]; cur; cur = cur->m_next )
      result += 1;
    return result;
  }  

  //------------------------------------------------------
  //Members
public:
  const uint convert( const _KEY& k ) const ; // Need IMPLEMENT  

protected:
  cell* hasher( const _KEY& k ) const
  {
    uint n = convert(k);
    if( n < m_buckets.size() )
      return m_buckets[n] ;
    return NULL;
  }

  //------------------------------------------------------
  //Elementos
protected:
  vector< cell* >  m_buckets;
  uint m_size ;  

	//-----------------------------------------------------------------------------
};
//_____________________________________________________________________________


//_____________________________________________________________________________
//Class Hash_it

template <typename _KEY,typename _DATA>

class Hash_it
//-----------------------------------------------------------------------------
{

protected:
  typedef HCell <_KEY,_DATA> cell ;
  typedef HashTable <_KEY,_DATA> hashtable ;  

public:
  friend class HashTable<_KEY,_DATA> ;    

protected:
	// Constructor & Desctructor  
  Hash_it( cell* cell , const hashtable* htable ) 
    :  m_cell(cell) , m_htable(htable) {}  

public:
  Hash_it( ) : m_cell(NULL) , m_htable(NULL) {}

  Hash_it( const hashtable* htable) : m_cell(NULL) , m_htable(htable) {}

  Hash_it( const Hash_it& it ) : m_cell(it.m_cell) , m_htable(it.m_htable) {}

  ~Hash_it () { clear(true);}

  const Hash_it& operator= ( const Hash_it& it )
  { m_cell = it.m_cell; m_htable = it.m_htable; return *this; }  
  
  //------------------------------------------------------
  //Operators
public:  
  bool operator() (const Hash_it& it) const { return isValid(); }
  const _DATA& operator*() const { assert( m_htable && m_cell ); return m_cell->data(); }
  _DATA& operator*() { assert( m_htable && m_cell ); return m_cell->data(); }  
  Hash_it& operator ++ ()
  {
    assert( m_htable && m_cell );
    const cell* old = m_cell;
    m_cell = m_cell->m_next;
    if (!m_cell) 
    {
      uint n = m_htable->convert(old->m_key) ;
      while (!m_cell && ++n < (uint)m_htable->m_buckets.size() )
        m_cell = m_htable->m_buckets[n];
    }
    return *this;
  }
  friend bool operator < ( const Hash_it& A , const Hash_it& B ) 
  {  assert(A.m_htable==B.m_htable);
    if( A.m_cell ) { if( B.m_cell ) return A.key() < B.key() ; else false; } return true; }
  bool operator > (const Hash_it& it) const {  assert( m_htable && m_cell ); return m_cell->m_key > it.m_cell->m_key; }

  friend bool operator== ( const Hash_it& A , const Hash_it& B ) 
  { if(A.m_cell) { if(B.m_cell) return (A.m_htable==B.m_htable) && (A.key()==B.key()) ;  else return false; }
    else  { if(B.m_cell) return false ; else return (A.m_htable==B.m_htable); } return false; }

  friend bool operator!= ( const Hash_it& A , const Hash_it& B )  { return !(A == B); }

  //------------------------------------------------------
  //Accessories
public:

  void clear(bool all = false) { if(all) m_htable = NULL; m_cell = NULL; }

  bool isValid() const {return m_htable!=NULL && m_cell!=NULL ;}  

  const _DATA& data() const { assert( m_htable && m_cell ); return m_cell->data(); }
  _DATA& data() { assert( m_htable && m_cell ); return m_cell->data(); }

  _KEY&  key() { assert( m_htable && m_cell ); return m_cell->key(); }
  const _KEY&  key() const { assert( m_htable && m_cell ); return m_cell->key(); }  

  static Hash_it null() { return Hash_it(); }
  //------------------------------------------------------
  //Elementos
private:  
  cell* m_cell ;
  const hashtable* m_htable; 

	//-----------------------------------------------------------------------------
};
//_____________________________________________________________________________


//_____________________________________________________________________________
//Class HCell

template <typename  _KEY,typename  _DATA>

class HCell
//-----------------------------------------------------------------------------
{
protected:
  friend class HashTable<_KEY,_DATA> ;
  friend class Hash_it<_KEY,_DATA> ;

protected:
	// Constructor & Desctructor    
  HCell ( const _KEY& key , const _DATA& data , HCell* next ) 
      : m_key(key) , m_DATA(data) , m_next(next)  {}
  HCell( const HCell& node ) : m_key(node.m_key),m_DATA(node.m_DATA),m_next(node.m_next){}
  ~HCell () {}
  //------------------------------------------------------
  //Set and Get Data
protected:  
  void key(const _KEY& k) { m_key = k; } 
  const _KEY key() const { return m_key; } 
  _KEY& key() { return m_key; } 
  const _DATA   data() const { return m_DATA; }
  _DATA& data() { return m_DATA; }
  void data( const _DATA& D ) { m_DATA = D; }
  HCell* next() { return m_next; }
  void next(HCell* N) { m_next = N; }

  //------------------------------------------------------
  //Elementos
private:  
  _KEY          m_key  ;
  _DATA       m_DATA ;
  HCell*       m_next ;
	//-----------------------------------------------------------------------------
};
//_____________________________________________________________________________


//_____________________________________________________________________________
#endif  //  _HASHTABLE_H_

