grep -i -o ei mobs.txt | wc -w
sed 's/ei/EI/g' mobs_in.txt | sed 's/Ei/EI/g' > MOBS_OUT.txt
