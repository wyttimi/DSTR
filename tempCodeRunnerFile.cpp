
// --- Memory estimation helpers ---
size_t estimateArrayMemoryCandidates(ArrayContainerCandidate& arr) {
    return arr.getSize() * sizeof(CandidateMatch);
}
size_t estimateLinkedListMemoryCandidates(LinkedListContainerCandidate& list) {
    return list.getSize() * (sizeof(CandidateMatch) + sizeof(void*));
}
