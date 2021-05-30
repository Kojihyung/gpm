#include "graph.h"
#include "common.h"
using namespace std;


int answer_check(Graph& data, Graph& query, vector<Vertex> ans, const int ans_size){
    for(auto elem = ans.begin(); elem != ans.end(); elem++){
        for(auto elem2 = elem+1; elem2 !=ans.end(); elem2++){
            if(elem == elem2){
                cout << "vertex used more than once. Vertex No. "<< *elem << endl;
                return 0;
            }
        }
    }

    for(Vertex i=0; i< ans_size;i++){
        for(Vertex iter = query.GetNeighborStartOffset(i); iter < query.GetNeighborEndOffset(i); iter++){
            if(!data.IsNeighbor(ans[i],ans[iter]))
            {
                return 0;
            }
        } 
    }
    return 1;
}

int main(int argc, char** argv){
    if (argc < 3) {
    std::cerr << "Usage: ./program <data graph file> <query graph file> "
                 "<anwser file>\n";
    return EXIT_FAILURE;
  }
  std::string data_file_name = argv[1];
  std::string query_file_name = argv[2];
  std::string answer_file_name = argv[3];

  Graph data(data_file_name);
  Graph query(query_file_name, true);

  std::ifstream fin(answer_file_name);

  if (!fin.is_open()) {
    std::cout << "Candidate set file " << answer_file_name << " not found!\n";
    exit(EXIT_FAILURE);
  }

  char type;
  size_t num_query_vertices;

  fin >> type >> num_query_vertices;
  std::vector<Vertex> ans;
  ans.resize(num_query_vertices);

  int count = 0;
  int right = 0;
  int wrong = 0;
  while (fin >> type) {
    if (type == 'a') {
        cout << "checking ans no. "<< count << endl;
        count++;
      Vertex id;
      for (size_t i = 0; i < num_query_vertices; ++i) {
        Vertex data_vertex;
        fin >> data_vertex;
        ans[i]=data_vertex;
      }

      if(answer_check(data,query,ans,num_query_vertices)){
          right++;
      }
      else{
          wrong++;
      }
    }
  }


  fin.close();
  std::cout << "right: " << right << endl;
  cout << "wrong: " << wrong << endl;
  cout << "rate: " << (float)right/(float)(right+wrong) << endl;
}
