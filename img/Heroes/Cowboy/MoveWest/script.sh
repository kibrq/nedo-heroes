find . -name '*.png' |
  sed -re 's/(Hero1FullRight(.\.png))/\1 \2/' | xargs -L1 mv
