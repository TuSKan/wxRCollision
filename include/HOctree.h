/**
 * @file    HOctree.h
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#ifndef _HOCTREE_H_
#define _HOCTREE_H_

#ifndef WIN32
#pragma interface
#endif // WIN32

#include "Utils.h"
#include "HashTable.h"
#include <vector>

//_____________________________________________________________________________
//Class HOcnode

class HOcnode
  //-----------------------------------------------------------------------------
{ 
  friend class HOctree;    
  //------------------------------------------------------
  // Constructor & Destructor  
public:
  HOcnode( cuint& np = 0 ) : m_isleaf(true) , m_npts(np) { }

  HOcnode( const HOcnode &node ) : m_isleaf(node.m_isleaf) , m_npts(node.m_npts) , m_trigs(node.m_trigs) { }

  ~HOcnode() {m_trigs.clear();}

  HOcnode& operator=( const HOcnode& node ) {m_isleaf=node.m_isleaf;m_npts=node.m_npts;m_trigs=node.m_trigs;return *this ;}
  //------------------------------------------------------
  //Set and Get Data
public:  
  inline uint npts() const { return m_npts; }
  inline bool isleaf() const { return m_isleaf; }
  inline std::vector<int>& trigs() {return m_trigs;}
  inline uint ntrigs() const { return m_trigs.size(); }
  //------------------------------------------------------
  //Elements
private:  
  bool  m_isleaf ;  
  uint  m_npts;

  std::vector<int> m_trigs;
  //-----------------------------------------------------------------------------
};
//_____________________________________________________________________________



//_____________________________________________________________________________
//Class HOctree

class HOctree
//-----------------------------------------------------------------------------
{ 
public:
  typedef HashTable<Key,HOcnode>::iterator iterator ; 
  typedef const iterator citerator ;
  //________________________________________________________
	// Constructor & Destructor  
public:
  HOctree() : m_level(0) {}
  ~HOctree() { m_tree.clear();}	
  //------------------------------------------------------
  //Accessories
public:
  void build( cBRep& surf , cuint& level );
  void build( const vector<Key>& keys , cuint& level , cuint& npt_max );
  
  iterator find_leaf( cKey& f ) const ;  
  inline iterator find_leaf( cPoint3D& P ) const
  {
    Key f = Point3D2Key( P , m_level ); 
    if( (m_tree.size() == 0) || f==0 ) return m_tree.end(); else return find_leaf( f ); 
  }  

  iterator find_node( cKey& f , const uint& level) const ;  
  inline iterator find_node( cPoint3D& P , cuint& level) const
    { uint lev = level; if( lev > m_level ) lev = m_level; Key f = Point3D2Key( P , lev ) ;
      if( (m_tree.size() == 0) || f==0 ) return m_tree.end(); else return find_node( f , lev); }  

  void find_neighbors(citerator& node,const uint& level,std::vector<iterator>& nlist) const;  
  inline void find_neighbors(citerator& node,std::vector<iterator>& nlist) const
  { if( node.isValid() ) find_neighbors(node,m_hist[0],nlist); }

  void find_inradius(citerator& node,const real& radius,const uint& level,std::vector<iterator>& rlist) const;      
  inline void find_inradius(citerator& node,creal& radius,std::vector<iterator>& rlist) const
  { if( node.isValid() && (radius > FLT_MIN) && (radius < 1.0f ) ) find_inradius(node,radius,m_hist[0],rlist); }  

  void optimize( cuint & type );

  void print_clocks() ;

  inline cuint* histograma() const { return m_hist; }
  
  inline int length( citerator& it ) const { return Key_Length( it.key() ) ; }

  //------------------------------------------------------
  // Recursive functions 

  iterator find_leaf_rec( const Point3D& P ) const ;

  iterator find_node_rec( const Point3D& P , const uint& level) const ; 

  void find_inradius_rec(const iterator& node,const float& radius, std::vector<iterator>& nlist) const;

  void find_neighbors_rec(const iterator& node,std::vector<iterator>& nlist) const;  

  int inMe_rec( const Cube& C , const Point3D& P ) const;
  inline int inMe_rec( const iterator& node , const Point3D& P ) const { return inMe_rec(cube(node),P); };
  //------------------------------------------------------
  //Members Aux
public:
  int inMe( citerator& node , Key k) const;    

  inline uint optm_level() const { return m_hist[0]; } 

  inline Cube cube(const iterator& node ) const { return Key2Cube(node.key(),Key_Length(node.key())); }

  void leafs_list( vector<iterator>& llist ) const ;
  //------------------------------------------------------
  //Get Tree Info
public:  
  inline iterator root() const { return m_tree.at(Key(1)); }

  inline iterator at( cKey& k ) const { return m_tree.at(k); }  

  inline iterator begin() const { return m_tree.begin(); }

  inline iterator end() const { return m_tree.end(); }

  inline uint size() const { return m_tree.size(); }

  inline void clear() { m_tree.clear();m_level=0;}    

  inline uint level() const { return m_level; }  

  void set_hash_func( cuint& hf ); 
  //------------------------------------------------------
  //Elements  
private:  
  uint m_level;
  uint m_hist[22];
  HashTable<Key,HOcnode> m_tree;  
	//-----------------------------------------------------------------------------
};
//_____________________________________________________________________________

//_____________________________________________________________________________
//_____________________________________________________________________________
#endif  //  _HOCTREE_H_
