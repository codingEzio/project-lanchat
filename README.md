> Run the following command if you have Docker installed

```bash
docker build . \
    --tag lanchat \
    --file ./Dockerfile

docker container \
    run -d \
    -p 80:80 -p 443:443 \
    --name inst-lanchat \
    lanchat
```
