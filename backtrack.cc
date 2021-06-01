/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <algorithm>
#include <vector>
using namespace std;

static Vertex *print_buf; //both print buffer and the record for currently visited data vertexs
static vector<Vertex> search_seq; //sequence to visit the query vertex

static char *is_visited; //is a query vertex visited?
static char *is_visited_data; //is a graph(data) vertex visited?

static int found_count; //success count. terminate when 10^5

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void rec_Backtrack(const Graph &data, const Graph &query, const CandidateSet &cs, int count)
{
  Vertex set = search_seq[count];
  /* end of the search: print */
  if (count == query.GetNumVertices())
  {
    found_count++;
    for(Vertex i = 0; i<count; i++){
      cout << print_buf[i] << endl;
    }
    if(found_count == 100000){
      cout << "good job" << endl;
      exit(0);
    }
    return;
  }

  for(Vertex candidate = 0; candidate< cs.GetCandidateSize(set); candidate++){

    if(is_visited_data[cs.GetCandidate(set,candidate)]==1){
      /* already used vertex */
      return;
    }

    for(int i=0; i<count; i++){
      /* check query connection for previous visited vertexes*/
      if(query.IsNeighbor(search_seq[i],set)){

        /* if they were connected on query, check if it is also connected in original graph */
        if(data.IsNeighbor(print_buf[search_seq[i]], cs.GetCandidate(set,candidate))){
          
          /* matched */
          print_buf[set] = cs.GetCandidate(set,candidate);
          is_visited_data[print_buf[set]] = 1;
          rec_Backtrack(data, query, cs, count+1);

          /* delete the visited flag after return*/
          is_visited_data[print_buf[set]] = 0;
          continue;
        }
        else{
          /* failed */
          continue;
        }
      }
    }
    return;
  }
  

}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs)
{
  print_buf = new Vertex[query.GetNumVertices()];
  is_visited = new char[query.GetNumVertices()];
  is_visited_data = new char[data.GetNumVertices()];

  for(int i=0; i<query.GetNumVertices(); i++){
    is_visited[i] = 0;
  }

  for(int i=0; i<data.GetNumVertices(); i++){
    is_visited_data[i] = 0;
  }
  std::cout << "t " << query.GetNumVertices() << "\n";

  Vertex curr;
  int max = 0;
  int neighbor_count = 0;

  /* query vertex search ordering: 
  0.the first query vertex to be visited is the one with the least element in its candidate set.
  1. next query vertex to be visited will be the one who has the most many neighbor in the already-visited vertex group. 
  2. if there is a tie in the number of neighbor, then choose the one with the least element
  */
  /* time complexity: query V ^2 */
  for (int iter = 0; iter < query.GetNumVertices(); iter++)
  {
    curr = 0;
    for (Vertex i = 0; i < query.GetNumVertices(); i++)
    {
      /* do not visit the query vertex more than once */
      if(is_visited[i]){
        continue;
      }

      /* count the number of neighbor */
      for (auto elem = search_seq.begin(); elem != search_seq.end(); elem++)
      {
        if (query.IsNeighbor(*elem, i))
          neighbor_count++;
      }
      
      /* choose the one with the most neighbor with the currently visited.*/
      if (neighbor_count > max)
      {
        curr = i;
        max = neighbor_count;
      }
      
      /* if the neighbor count is tie, than choose the one with less candidate. */
      /* always tie in the first iteration, since neighbor_count is zero */
      else if (neighbor_count == max)
      {
        if (cs.GetCandidateSize(i) < cs.GetCandidateSize(curr))
        {
          curr = i;
        }
      }
    }
    neighbor_count = 0;
    is_visited[curr] = 1;
    search_seq.push_back(curr);
  }

  for(int i =0; i<query.GetNumVertices();i++){
    /* debugging */
    cout << "query sequence" + i + search_seq[i] << endl;
  }
  /* start the backtracking */
  rec_Backtrack(data, query, cs, 0);
}
