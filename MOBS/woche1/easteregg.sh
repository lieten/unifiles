grep -i -o ei mobs.txt | wc -w
sed 's/ei/EI/g' mobs.txt | sed 's/Ei/EI/g' > MOBS.txt
