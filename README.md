# SearchEngine
Simplified web search engine

This a web search engine that operates the following way:
  scraper_converter.py starts with a given starting link and parses all the words, contained on that webpage, and links outgoing from the page. This operation continues recursively up to some predefined depth. 
  After the process is finished is over, and all required material is collected, we can manually run the main.cpp file. Right after this, function, evaluating ranks of the pages will be called. It will insert all of the words into a Trie data structure for fast non-linear access. Now, we are ready to search, by just running the main.cpp file. It is able to find the requests consisting of 1 word (for now...), and output the resulting websites in order of relevance. In case nothing is found, the appropriate message will appear.


  scraper_converter.py contains functions that create adjacency(accessibility) matrix of the available portion of web. Due to speed limitation of python, this routine is performed concurrently.
  
  PageRank.h (included into main.cpp) contains definitions of ranking functions. The ranking is performed with the following approach: The website is considered highly ranked if other highly ranked pages reference it. Basically, the websites and links between them is a graph data structure, represented with the adjacency matrix.  Ranks are calculated with some basic arithmetics. After some points, these ranks converge to definite values. However there are important things to point out:
  
  1) The code makes sure that the nodes(webpages) that are not linked to sequence of nodes in any way, are also considered in calculation (with a random surfer model). 100*(1-Beta) times out of 100 (Beta=0.85 in this implemetation), the "surfer" does not follow the links, but jumps on a random page.
     
  2) There is an issue of nodes that have no outgoing links. Such nodes, if encountered, break the matrix calculation and, as a result, total probabilities of nodes do not add up to 1. To resolve this, we must make sure that we can move to some other node when arriving at a "linkless" node. It can be done in two ways : A) We either evoke "random surfer" right away, or B) link this node to every other node in the graph. This model is using B) due to its implemetation simplicity. (However, A) is more computionally efficient, because adjacency matrix will not be as dense compared to A)).


This example code starts the search from METU Computer Engineering department's page https://ceng.metu.edu.tr/undergraduate-curriculum (can be changed to start from another region of the internet). The depth of search is 1 (could be increased to consider a bigger network of webpages).
