# Summary of Resources

## 1. Hemorrhagic Stroke Lesion Segmentation Using a 3D U-Net with Squeeze-and-Excitation Blocks
*Abramova, V., Clèrigues, A., Quiles, A., Garcia Figueredo, D., Silva, Y., Pedraza, S., Oliver, A., Lladó, X. — Computerized Medical Imaging and Graphics, Vol. 90, 101908, 2021. DOI: 10.1016/j.compmedimag.2021.101908*

**Problem:** Manual CT volume segmentation for hemorrhagic stroke is slow and variable; automated tools provide fast, objective lesion metrics.

**Method:** A 3D U-Net backbone captures volumetric spatial context with skip connections. Squeeze-and-Excitation (SE) blocks add channel-wise attention, restrictive patch sampling counters severe class imbalance, and symmetric modality augmentation leverages brain symmetry to highlight lesions and suppress false positives (e.g., intraventricular hemorrhage).

**Results & Takeaway:** Evaluated on 76 clinical CT cases via 5-fold cross-validation, the model achieves a mean DSC of ~0.86 in ~17 seconds per volume. Domain-specific enhancements (attention, symmetry, sampling) improve over a plain 3D U-Net.

## 2. Panoptic Segmentation (arXiv:1801.00868)
*Kirillov, A., He, K., Girshick, R., Rother, C., Dollár, P. — CVPR 2019*

**Motivation:** Semantic segmentation (pixel-level class labels for amorphous "stuff") and instance segmentation (masks for countable "things") had evolved separately, with different methods and metrics, so neither gave a complete picture of the scene.

**Task Definition:** Unifies both into one task: each pixel receives a single non-overlapping class label, plus a unique instance ID for countable objects.

**Panoptic Quality (PQ):** Predicted and ground-truth segments are matched when IoU > 0.5. **PQ = SQ × RQ**, where Segmentation Quality (SQ) is the mean IoU of matched segments and Recognition Quality (RQ) is an F1 score for detection. The metric applies uniformly to both stuff and things.

**Empirical Study & Impact:** Measured human vs. machine performance on existing datasets (Cityscapes, ADE20K, Mapillary Vistas), finding machines lag mainly in recognition. Inspired unified architectures such as Panoptic FPN, Panoptic-DeepLab, and Mask2Former.

## 3. Cityscapes Dataset (Examples page)

**Overview:** A benchmark dataset for urban scene understanding captured via a vehicle-mounted stereo camera across **50 cities** (mainly Germany and neighbouring countries), containing 5,000 finely annotated images (19 evaluation classes) and 20,000 coarsely annotated images at 2048×1024 resolution.

**Relevance:** Demonstrates pixel-level polygon masks, video sequences, and privacy protection (blurred faces/plates). Highlights domain-shift challenges when transferring models trained on European cities to unstructured traffic environments (e.g., Karachi).

## Overall Takeaway
Segmentation progress relies on strong encoder–decoder backbones (U-Net and its variants), unified task formulations (panoptic segmentation with PQ), and high-quality, ethically collected annotated datasets. CVPR and ICML are the main venues where such advances appear — the state-of-the-art techniques presented there are covered in Part 4.

## References
1. Abramova, V. et al. (2021). *Hemorrhagic stroke lesion segmentation using a 3D U-Net with squeeze-and-excitation blocks.* Computerized Medical Imaging and Graphics, 90, 101908. https://www.sciencedirect.com/science/article/pii/S0895611121000574
2. Kirillov, A., He, K., Girshick, R., Rother, C., Dollár, P. (2019). *Panoptic Segmentation.* CVPR. https://arxiv.org/abs/1801.00868
3. Cordts, M. et al. (2016). *The Cityscapes Dataset for Semantic Urban Scene Understanding.* CVPR. https://www.cityscapes-dataset.com/examples/
4. CVPR 2025. https://cvpr.thecvf.com/Conferences/2025
5. ICML 2024 Call for Papers. https://icml.cc/Conferences/2024/CallForPapers
