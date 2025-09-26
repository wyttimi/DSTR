    auto* jobNode = jobsList.getHead();
    while (jobNode) {
        Job jb = jobNode->data;
        string jobText = toLowerStr(jb.getTitle() + " " + jb.getDescription());

        if (jobText.find(keyword) != string::npos) {
            cout << "\n[LinkedList] Job: \"" << jb.getTitle() << "\"" << endl;

            clock_t start = clock();
            LinkedListContainer<CandidateMatch> listCandidates;

            auto* resNode = resumesList.getHead();
            while (resNode) {
                Resume res = resNode->data;
                int score = JobMatcher::calculateScore(jb, res);

                if (score > 0) listCandidates.insert({res, score});
                resNode = resNode->next;
            }
            clock_t end = clock();
            double listTime = double(end - start) / CLOCKS_PER_SEC;

            // --- Print Top 5 ---
            int shown = 0;
            auto* candNode = listCandidates.getHead();
            while (candNode && shown < 5) {
                cout << "   -> Candidate Resume: " 
                    << candNode->data.resume.getDescription() << endl;
                cout << "      Score: " << candNode->data.score << endl;

                candNode = candNode->next;
                shown++;
            }
            cout << "   [Total Matches: " << listCandidates.getSize()
                << " | Time: " << listTime << " sec]" << endl;
        }
        jobNode = jobNode->next;
    }