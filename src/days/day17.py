import heapq

def get_data(fname):
  
  data = open(fname).read().strip().split('\n')
  grid = [[c for c in row] for row in data]
  rows = len(grid)
  cols = len(grid[0])
  return grid, rows, cols


def score(grid, rows, cols, part2):
  
  queue = [(0, 0, 0, -1e6, -1e6)]
  dict = {}

  while queue:

    loss, r, c, prev_dir, prev_num_straight = heapq.heappop(queue)
    if (r, c, prev_dir, prev_num_straight) in dict:
      continue

    dict[(r, c, prev_dir, prev_num_straight)] = loss
    for i, (dr, dc) in enumerate([[-1, 0], [0, 1], [1, 0], [0, -1]]):
      cur_row = r + dr
      cur_col = c + dc
      cur_dir = i
      num_straight = (1 if cur_dir != prev_dir else prev_num_straight + 1)
      
      #No moving backward
      if not ((cur_dir + 2) % 4 != prev_dir):
        continue
      
      if part2:
        if not (num_straight <=10 and (cur_dir == prev_dir or prev_num_straight >= 4 or prev_num_straight == -1e6)):
          continue
      else:
        if num_straight > 3:
          continue

      if 0 <= cur_row < rows and 0 <= cur_col < cols:
        cost = int(grid[cur_row][cur_col])
        heapq.heappush(queue, (loss + cost, cur_row, cur_col, cur_dir, num_straight))

  best = 1e6
  for (r, c, prev_dir, prev_num_straight), v in dict.items():
    if r == rows - 1 and c == cols - 1:
      best = min(best, v)
  return best


def part1():
  
  fname = "data/day17.txt"
  grid, rows, cols = get_data(fname)
  print(score(grid, rows, cols, False))


def part2():
  
  fname = "data/day17.txt"
  grid, rows, cols = get_data(fname)
  print(score(grid, rows, cols, True))

part1()
part2()