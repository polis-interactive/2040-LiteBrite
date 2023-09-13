import { useAuth0 } from "./useAuth"

export const useApiFetch: typeof useFetch = async (request: string, opts?)  => {
    const config = useRuntimeConfig()
    const auth0 = await useAuth0();
    const token = await auth0.value.getTokenSilently();
    const fetchOptions = { 
        baseURL: config.public.apiUrl,
        ...opts,
        headers: {
            ...(opts?.headers ?? {}),
            Authorization: `Bearer ${token}`
        }
    };
    return useFetch(request, fetchOptions)
  }