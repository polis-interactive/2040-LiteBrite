
export const useBaseUrlFetch: typeof useFetch = (request, opts?) => {
    const config = useRuntimeConfig()
    if (!!config.public.baseURL) {
        return useFetch(request, { 
            baseURL: config.public.baseURL,
            ...opts 
        })
    } else {
        return useFetch(request, opts)
    }
  }