            cout << "   → Total " << (mode == 1 ? "candidates" : "jobs") 
                << " matched: " << cand.getSize() << "\n";
            mem = estimateLinkedMemory(cand);
            cand.clear();