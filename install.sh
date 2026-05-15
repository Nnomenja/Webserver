# Install Siege locally in ~/.local
mkdir -p ~/.local/src && cd ~/.local/src \
&& wget -q https://github.com/JoeDog/siege/releases/download/v4.1.8/siege-4.1.8.tar.gz \
&& tar -xzf siege-4.1.8.tar.gz \
&& cd siege-4.1.8 \
&& ./configure --prefix=$HOME/.local \
&& make \
&& make install \
&& echo 'export PATH=$HOME/.local/bin:$PATH' >> ~/.bashrc \
&& source ~/.bashrc \
&& siege --version
