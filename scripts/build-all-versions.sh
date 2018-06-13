# This script builds each release tag of the project
# And saves the binary in an output folder

# Path Constants
CURRENT_DIR=`pwd`
OUTPUT_DIR=$CURRENT_DIR/output
PROJECT_DIR=$1
TAGS_FILE=git_tag_names.txt

mkdir $OUTPUT_DIR
cd $PROJECT_DIR/.git
GIT_DIR=`pwd`

cd $CURRENT_DIR
# Get git tags and save them to file
git --git-dir $GIT_DIR tag > $TAGS_FILE
# Loop through tags in file
while read p; do
  # Run on each tag
  git --git-dir $GIT_DIR checkout $p
  make || make clean && make
  cp assign3_part2 $OUTPUT_DIR/$p
done <$TAGS_FILE
