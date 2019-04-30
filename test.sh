./format -input ../../data/ydir_train.txt -prefix train
./format -input ../../data/ydir_test.txt -vocab_in train.vocab -test -prefix test

./warplda --prefix train --k 100 --niter 300

cat train.info.full.txt

./warplda --prefix test --model train.model --inference -niter 40 --perplexity 10
