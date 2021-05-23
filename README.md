Etapes pour l'installation du code :

Entrez les commandes suivantes.

    sudo apt install python3-yaml
    sudo apt install libyaml-dev
    sudo apt install pkg-config
    sudo apt install libyaml-0-2

Au cas où vous ne l'auriez pas fait, ces 2 commandes vous permettrons de lier votre shell a github.

    git config --global user.name <username>
    git config --global user.email <user email>

Ensuite :

    git clone https://github.com/Tucobad/Cocktail_bar.git
    Dans le répertoire Cocktail_bar :
    	make init
    	make
	./bin/cocktail_bar (pour executer)
