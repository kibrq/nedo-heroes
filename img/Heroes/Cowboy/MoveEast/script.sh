find . -name '*.png' |
  sed -re 's/\.\/(.*)/\1 ..\/MoveWest\/\1/' | xargs -L1 bash -c 'convert $0 -flop $1'
