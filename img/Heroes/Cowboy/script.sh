find . -name 'Hero*' |
  xargs -L1 -I {} convert {} -resize 100x100 {} 
