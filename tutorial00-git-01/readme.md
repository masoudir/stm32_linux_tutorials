# Very short introduction to git

We will do this by examples, to be really understandable for beginners.

# Requirements

You can install git on your OS. Then everywhere you want to access to git on your local storage, you can open a terminal there (in windows by write clicking and select git bash here).

## Example01 : cloning + adding changes + commiting + pushing

You need to create an example git repository in your github. We will use our github address in this example. Then open a terminal somewhere in your computer:

    git clone https://github.com/masoudir/stm32_linux_tutorials

Then go to that cloned folder:

    cd stm32_linux_tutorials

We call this path as parent folder in git. It is recommended to use this path for git.
If you want to add a new file, you can create "MyFile.txt" and use this command:

    git add .

Now all of your changes are added to cash and need to be committed:

    git commit -m "My message"

Then if you want to send your local changes to remote server:

    git push

## Example02: adding new local branch + sending new branch to remote server

You need to create an example git repository in your github. We will use our github address in this example. Then open a terminal somewhere in your computer:

    git clone https://github.com/masoudir/stm32_linux_tutorials

Then go to that cloned folder:

    cd stm32_linux_tutorials

We call this path as parent folder in git. It is recommended to use this path for git.
Then for creating new local branch:

    git branch -b new_branch_name

Now, you are inside your new branch. If you want to go back to main branch again:

    git checkout main

You can do this "checkout" for every branches or commits.


## Example03: removing a local branch + see the logs

Following previous example, you can use this command to see the list of the branches:

    git branch

The star sign shows which branch you are in.
Then if you want to delete "new_branch_name" branch:

    git branch -D new_branch_name

If you want to see the logs:

    git log

Note that, if your terminal screen is not enough big, then you need to use arrow keys to see other logs and then type q for quit.

There is another way to see the logs shortly:

    git reflog

If you want to go back to your previous commits, copy the commit hash id and then:

    git checkout <commit_hash_id>

    