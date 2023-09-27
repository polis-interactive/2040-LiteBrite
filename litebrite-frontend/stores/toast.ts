
import { defineStore } from 'pinia'
import { Toast } from '~/lib/domain/toast';

export const useToastStore = defineStore('toast', () => {

    /* state */
    const toastQueue: Ref<Array<Toast>> = ref([]);
    const toastId = ref(0)

    const theSizeOfSnackbar = 64
    const offsetBottom = 16;

    /* actions */
    const pushQueue = (toast: Toast) => {
        toast.id = ++toastId.value;
        toast.position = theSizeOfSnackbar * toastQueue.value.length + offsetBottom
        toastQueue.value.push(toast);
    }

    const popElement = (id: number) => {
      const removedIdx = toastQueue.value.findIndex(x => x.id === id)
      toastQueue.value.splice(removedIdx, 1)
      toastQueue.value.forEach((x, idx) => (x.position = theSizeOfSnackbar * idx + offsetBottom))
    }

    return { toastQueue, pushQueue, popElement }

});