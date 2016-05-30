#define STRIP_FLAG_HELP 0

#include <gflags/gflags.h>

#include "Bigraph.hpp"
#include "Vocab.hpp"
#include "Utils.hpp"
#include <exception>
#include <algorithm>

DEFINE_string(prefix, "./prefix", "prefix of temporary files");
DEFINE_string(vocab, "", "vocabulary file");
DEFINE_string(input, "", "input file");
DEFINE_string(output, "", "output file");
DEFINE_string(method, "", "text2bin");
//DEFINE_string(method, "", "text2bin/libsvm2bin");
DEFINE_int32(skip, 2, "skip num of words at first of each line");

void text_to_bin(std::string in, std::string out, std::string vocab_name, int skip)
{
    Vocab v;
    v.load(vocab_name);
    int doc_id = 0;
    std::vector<std::pair<TUID, TVID>> edge_list;
    bool success = ForEachLinesInFile(in, [&](std::string line)
    {
        std::istringstream sin(line);
        std::string w;
        for (int i = 0; sin>>w; i++)
        {
            if (i >= skip)
            {
                int word_id = v.addWord(w);
                edge_list.emplace_back(doc_id, word_id);
            }
        }
        doc_id++;
    });
    if (!success)
        throw std::runtime_error(std::string("Failed to input file ") + in);
    std::vector<TVID> new_vid(v.nWords());
    for (unsigned i = 0; i < new_vid.size(); i++)
        new_vid[i] = i;
    std::random_shuffle(new_vid.begin(), new_vid.end());
    v.RearrangeId(new_vid.data());
    v.store(vocab_name);
    for (auto &e : edge_list)
        e.second = new_vid[e.second];
    Bigraph::Generate(out, edge_list);
}

int main(int argc, char** argv)
{
    gflags::SetUsageMessage("Usage : ./transform [ flags... ]");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	if (FLAGS_method == "text2bin")
	{
		if (FLAGS_input.empty())
			FLAGS_input = FLAGS_prefix + ".txt";
		if (FLAGS_output.empty())
			FLAGS_output = FLAGS_prefix + ".bin";
		if (FLAGS_vocab.empty())
			FLAGS_vocab = FLAGS_prefix + ".vocab";
        text_to_bin(FLAGS_input, FLAGS_output, FLAGS_vocab, FLAGS_skip);
	}
    #if 0
    else if (FLAGS_method == "libsvm2bin")
	{

		if (FLAGS_input.empty())
			FLAGS_input = FLAGS_prefix + ".libsvm";
		if (FLAGS_output.empty())
			FLAGS_output = FLAGS_prefix + ".bin";
	}
    #endif
    else
    {
        throw std::runtime_error(std::string("Unknown method " + FLAGS_method));
    }
	return 0;
}