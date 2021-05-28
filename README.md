# Dijkstra_SP

- Based on **Adjacency Matrix**
    In worst case, O(|V|^2+|E|)-time.
    And it uses O(|V|^2)-spaces.
    <img width="265" alt="스크린샷 2021-05-28 오후 3 28 43" src="https://user-images.githubusercontent.com/30382262/119939895-75c35780-bfc9-11eb-920f-dfc19a719ae9.png">
    it uses approximately 130MB of memory in the test4.


- Based on **Priority Queue**
    In worst case, O(|E| * |log|V|)-time.
    And it only uses O(|E|)-spaces.
    <img width="265" alt="스크린샷 2021-05-28 오후 3 27 59" src="https://user-images.githubusercontent.com/30382262/119939889-73f99400-bfc9-11eb-84b3-b52dcdb9a515.png">
    it uses approximately 20MB of memory in the test4.

When there are very many edges, O(|E|) ∈  O(|V|^2), so it is appropriate to use **Adjacency Matrix** in terms of execution time, 
but it is advantageous to always use **Priority Queue** from a spacial point of view.

On the other hand, if the number of edges is very sparse, it is recommanded to use **Priority Queue** from any point of view.