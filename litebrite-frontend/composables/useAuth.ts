import { Auth0Client, createAuth0Client } from '@auth0/auth0-spa-js';

const useAuth0Client = () => {
    return useState<Auth0Client | null>("auth0Client", () => null);
};

export const useAuth0 = async (): Promise<Ref<Auth0Client>> => {
    const client = useAuth0Client();
    if (client.value === null) {
        const config = useRuntimeConfig();
        client.value = await createAuth0Client(config.public.auth0Config);
    }
    return client as Ref<Auth0Client>;
}